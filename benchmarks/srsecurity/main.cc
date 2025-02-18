#include <iostream>
#include <mutex>
#include <thread>

#include "common/duration.h"
#include "engine.h"

#include "../test_data/request.h"

constexpr uint32_t max_test_count = 10000000;
uint32_t test_count = 0;
std::mutex mutex;

void thread_func(SrSecurity::Engine& engine) {
  while (true) {
    Request request;
    SrSecurity::ConnectionExtractor conn_extractor =
        [&](std::string_view& downstream_ip, short& downstream_port, std::string_view& upstream_ip,
            short& upstream_port) {
          downstream_ip = request.downstream_ip_;
          downstream_port = request.downstream_port_;
          upstream_ip = request.upstream_ip_;
          upstream_port = request.upstream_port_;
        };

    SrSecurity::UriExtractor uri_extractor = [&](std::string_view& method, std::string_view& path,
                                                 std::string_view& protocol,
                                                 std::string_view& version) {
      method = request.method_;
      path = request.path_;
      protocol = request.protocol_;
      version = request.version_;
    };

    SrSecurity::HeaderExtractor request_header_extractor = [&](const std::string& key) {
      std::vector<std::string_view> result;
      auto range = request.request_headers_.equal_range(key);
      for (auto iter = range.first; iter != range.second; ++iter) {
        result.emplace_back(iter->second.data(), iter->second.length());
      }

      if (result.size() > 0) {
        return result[0];
      } else {
        return std::string_view();
      }
    };

    SrSecurity::BodyExtractor request_body_extractor =
        [&]() -> const std::vector<std::string_view>& { return request.request_body_; };

    auto t = engine.makeTransaction();
    t->processConnection(conn_extractor);
    t->processUri(uri_extractor);
    t->processRequestHeaders(request_header_extractor);

    std::lock_guard<std::mutex> lock(mutex);
    if (test_count >= max_test_count) {
      break;
    }
    ++test_count;
  }
}

int main(int argc, const char* argv[]) {
  SrSecurity::Engine engine;
  std::expected<bool, std::string> result = engine.loadFromFile(
      "test/test_data/waf-conf/coreruleset/rules/REQUEST-901-INITIALIZATION.conf");
  if (!result.has_value()) {
    std::cout << "Load rules error: " << result.error() << std::endl;
    return 1;
  }

  engine.init();

  std::vector<std::thread> threads;
  SrSecurity::Common::Duration duration;
  for (int i = 0; i < std::thread::hardware_concurrency(); ++i) {
    threads.emplace_back(std::thread(thread_func, std::ref(engine)));
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