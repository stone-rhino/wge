#include <iostream>
#include <mutex>
#include <thread>

#include "common/duration.h"
#include "modsecurity/modsecurity.h"
#include "modsecurity/rules_set.h"
#include "modsecurity/transaction.h"

#include "../test_data/request.h"

constexpr uint32_t max_test_count = 10000000;
uint32_t test_count = 0;
std::mutex mutex;

void logCb(void* data, const void* message) {}

void thread_func(modsecurity::ModSecurity& engine, modsecurity::RulesSet& rules_set) {
  while (true) {
    Request request;
    modsecurity::Transaction t(&engine, &rules_set, nullptr);

    t.processConnection(request.downstream_ip_.c_str(), request.downstream_port_,
                        request.upstream_ip_.c_str(), request.upstream_port_);
    t.processURI(request.path_.c_str(), request.protocol_.c_str(), request.version_.c_str());

    for (auto& [key, value] : request.request_headers_) {
      t.addRequestHeader(key, value);
    }
    t.processRequestHeaders();

    std::lock_guard<std::mutex> lock(mutex);
    if (test_count >= max_test_count) {
      break;
    }
    ++test_count;
  }
}

int main(int argc, const char* argv[]) {
  modsecurity::ModSecurity engine;
  modsecurity::RulesSet rules_set;

  engine.setServerLogCb(logCb);
  int result = rules_set.loadFromUri(
      "test/test_data/waf-conf/coreruleset/rules/REQUEST-901-INITIALIZATION.conf");
  if (result == -1) {
    std::cout << "Load rules error: " << rules_set.getParserError() << std::endl;
    return 1;
  }

  std::vector<std::thread> threads;
  SrSecurity::Common::Duration duration;
  for (int i = 0; i < std::thread::hardware_concurrency(); ++i) {
    threads.emplace_back(std::thread(thread_func, std::ref(engine), std::ref(rules_set)));
  }
  for (auto& thread : threads) {
    thread.join();
  }

  duration.stop();
  std::cout << "Total time: " << duration.milliseconds() << "ms" << std::endl;
  std::cout << std::fixed << std::setprecision(3)
            << "QPS:" << 1000.0 * test_count / duration.milliseconds() << std::endl;

  return 0;
}