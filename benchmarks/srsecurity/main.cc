#include <iostream>
#include <mutex>
#include <thread>

#include <unistd.h>

#include "common/duration.h"
#include "engine.h"

#include "../test_data/request.h"

uint32_t test_count = 0;
std::mutex mutex;

void thread_func(SrSecurity::Engine& engine, uint32_t max_test_count) {
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
    t->processRequestHeaders(request_header_extractor, nullptr);

    std::lock_guard<std::mutex> lock(mutex);
    if (test_count >= max_test_count) {
      break;
    }
    ++test_count;
  }
}

void usage(void);
int main(int argc, char* argv[]) {
  // Thread count, default is the number of CPU cores
  uint32_t concurrency = std::thread::hardware_concurrency();
  // The maximum requests number of tests, default 10000000
  uint32_t max_test_count = 10000000;

  // Parse command line arguments
  int opt;
  while ((opt = getopt(argc, argv, "c:n:h")) != -1) {
    switch (opt) {
    case 'c':
      try {
        concurrency = std::stoi(optarg);
      } catch (...) {
        std::cout << "Invalid concurrency value" << std::endl;
        usage();
        return 1;
      }
      break;
    case 'n':
      try {
        max_test_count = std::stoi(optarg);
      } catch (...) {
        std::cout << "Invalid test count value" << std::endl;
        usage();
        return 1;
      }
      break;
    case 'h':
    default:
      usage();
      return 0;
    }
  }

  // Load rules
  SrSecurity::Engine engine;
  std::expected<bool, std::string> result = engine.loadFromFile(
      "test/test_data/waf-conf/coreruleset/rules/REQUEST-901-INITIALIZATION.conf");
  if (!result.has_value()) {
    std::cout << "Load rules error: " << result.error() << std::endl;
    return 1;
  }
  engine.init(spdlog::level::off);

  // Start benchmark
  std::vector<std::thread> threads;
  SrSecurity::Common::Duration duration;
  for (int i = 0; i < concurrency; ++i) {
    threads.emplace_back(std::thread(thread_func, std::ref(engine), max_test_count));
  }

  // Wait for all threads to finish
  for (auto& thread : threads) {
    thread.join();
  }

  // Print benchmark result
  duration.stop();
  std::cout << "Test count: " << test_count << std::endl;
  std::cout << "Total time: " << duration.milliseconds() << "ms" << std::endl;
  std::cout << std::fixed << std::setprecision(3)
            << "QPS:" << 1000.0 * max_test_count / duration.milliseconds() << std::endl;

  return 0;
}

void usage() {
  std::cout << R"(USAGE: modsecurity_benchmark [-c concurrency] [-n test count]
       -c concurrency
               thread count, default is the number of CPU cores
       -n test count
               the maximum requests number of tests, default 10000000

)" << std::endl;
}