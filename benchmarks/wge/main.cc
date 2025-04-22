#include <array>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

#include <unistd.h>

#include "common/duration.h"
#include "engine.h"

#include "../test_data/test_data.h"

uint32_t test_count = 0;
std::mutex mutex;

void process(Wge::Engine& engine, const HttpInfo& http_info) {
  Wge::HeaderFind request_header_find = [&](const std::string& key) {
    std::vector<std::string_view> result;
    auto range = http_info.request_headers_.equal_range(key);
    for (auto iter = range.first; iter != range.second; ++iter) {
      result.emplace_back(iter->second.data(), iter->second.length());
    }

    if (result.size() > 0) {
      return result[0];
    } else {
      return std::string_view();
    }
  };

  Wge::HeaderTraversal request_header_traversal =
      [&](Wge::HeaderTraversalCallback callback) {
        for (auto& [key, value] : http_info.request_headers_) {
          if (!callback(key, value)) {
            break;
          }
        }
      };

  Wge::BodyExtractor request_body_extractor = [&]() -> const std::vector<std::string_view>& {
    return http_info.request_body_;
  };

  Wge::HeaderFind response_header_find = [&](const std::string& key) {
    std::vector<std::string_view> result;
    auto range = http_info.response_headers_.equal_range(key);
    for (auto iter = range.first; iter != range.second; ++iter) {
      result.emplace_back(iter->second.data(), iter->second.length());
    }

    if (result.size() > 0) {
      return result[0];
    } else {
      return std::string_view();
    }
  };

  Wge::HeaderTraversal response_header_traversal =
      [&](Wge::HeaderTraversalCallback callback) {
        for (auto& [key, value] : http_info.response_headers_) {
          if (!callback(key, value)) {
            break;
          }
        }
      };

  Wge::BodyExtractor response_body_extractor =
      [&]() -> const std::vector<std::string_view>& { return http_info.response_body_; };

  auto t = engine.makeTransaction();
  t->processConnection("192.168.1.100", 20000, "192.168.1.200", 80);
  t->processUri(http_info.request_uri_, http_info.request_method_, http_info.request_version_);
  t->processRequestHeaders(request_header_find, request_header_traversal,
                           http_info.request_headers_.size(), [](const Wge::Rule& rule) {
                             // std::cout << rule.getId() << std::endl;
                           });
  t->processRequestBody(request_body_extractor, [](const Wge::Rule& rule) {
    // std::cout << rule.getId() << std::endl;
  });
  t->processResponseHeaders(http_info.response_status_code_, http_info.response_protocol_,
                            response_header_find, response_header_traversal,
                            http_info.response_headers_.size(), [](const Wge::Rule& rule) {
                              // std::cout << rule.getId() << std::endl;
                            });
  t->processResponseBody(response_body_extractor, [](const Wge::Rule& rule) {
    // std::cout << rule.getId() << std::endl;
  });
}

void thread_func(Wge::Engine& engine, uint32_t max_test_count,
                 const TestData& test_data_white, const TestData& test_data_black) {
  while (true) {
    auto& white_data = test_data_white.getHttpInfos();
    auto& black_data = test_data_black.getHttpInfos();

    for (auto& http_info : white_data) {
      process(engine, http_info);
    }
    for (auto& http_info : black_data) {
      process(engine, http_info);
    }

    std::lock_guard<std::mutex> lock(mutex);
    test_count += white_data.size() + black_data.size();
    if (test_count >= max_test_count) {
      break;
    }
  }
}

void usage(void);
int main(int argc, char* argv[]) {
  // Thread count, default is the number of CPU cores
  uint32_t concurrency = std::thread::hardware_concurrency();
  // The maximum requests number of tests, default 10000000
  uint32_t max_test_count = 100000;

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

  // Load Test data
  TestData test_data_white(TestData::Type::White);
  if (test_data_white.getHttpInfos().empty()) {
    std::cout << "Load white test data error" << std::endl;
    return 1;
  }

  TestData test_data_black(TestData::Type::Black);
  if (test_data_black.getHttpInfos().empty()) {
    std::cout << "Load black test data error" << std::endl;
    return 1;
  }

  // Load rules
  Wge::Engine engine(spdlog::level::trace);
  std::expected<bool, std::string> result;
  std::vector<std::string> rule_files = {
      "test/test_data/engin-setup.conf",
      "test/test_data/crs-setup.conf",
      "test/test_data/coreruleset/rules/REQUEST-901-INITIALIZATION.conf",
      "test/test_data/coreruleset/rules/REQUEST-905-COMMON-EXCEPTIONS.conf",
      "test/test_data/coreruleset/rules/REQUEST-911-METHOD-ENFORCEMENT.conf",
      "test/test_data/coreruleset/rules/REQUEST-913-SCANNER-DETECTION.conf",
      "test/test_data/coreruleset/rules/REQUEST-920-PROTOCOL-ENFORCEMENT.conf",
      "test/test_data/coreruleset/rules/REQUEST-921-PROTOCOL-ATTACK.conf",
      "test/test_data/coreruleset/rules/REQUEST-922-MULTIPART-ATTACK.conf",
      "test/test_data/coreruleset/rules/REQUEST-930-APPLICATION-ATTACK-LFI.conf",
      "test/test_data/coreruleset/rules/REQUEST-931-APPLICATION-ATTACK-RFI.conf",
      "test/test_data/coreruleset/rules/REQUEST-932-APPLICATION-ATTACK-RCE.conf",
      "test/test_data/coreruleset/rules/REQUEST-933-APPLICATION-ATTACK-PHP.conf",
      "test/test_data/coreruleset/rules/REQUEST-934-APPLICATION-ATTACK-GENERIC.conf",
      "test/test_data/coreruleset/rules/REQUEST-941-APPLICATION-ATTACK-XSS.conf",
      "test/test_data/coreruleset/rules/REQUEST-942-APPLICATION-ATTACK-SQLI.conf",
      "test/test_data/coreruleset/rules/"
      "REQUEST-943-APPLICATION-ATTACK-SESSION-FIXATION.conf",
      "test/test_data/coreruleset/rules/REQUEST-944-APPLICATION-ATTACK-JAVA.conf",
      "test/test_data/coreruleset/rules/REQUEST-949-BLOCKING-EVALUATION.conf",
      "test/test_data/coreruleset/rules/RESPONSE-950-DATA-LEAKAGES.conf",
      "test/test_data/coreruleset/rules/RESPONSE-951-DATA-LEAKAGES-SQL.conf",
      "test/test_data/coreruleset/rules/RESPONSE-952-DATA-LEAKAGES-JAVA.conf",
      "test/test_data/coreruleset/rules/RESPONSE-953-DATA-LEAKAGES-PHP.conf",
      "test/test_data/coreruleset/rules/RESPONSE-954-DATA-LEAKAGES-IIS.conf",
      "test/test_data/coreruleset/rules/RESPONSE-955-WEB-SHELLS.conf",
      "test/test_data/coreruleset/rules/RESPONSE-959-BLOCKING-EVALUATION.conf",
      "test/test_data/coreruleset/rules/RESPONSE-980-CORRELATION.conf",
  };

  // Set the blocking_paranoia_level
  result = engine.load(
      R"(SecAction "id:205, phase:1,nolog,pass,t:none,setvar:tx.blocking_paranoia_level=4")");
  if (!result.has_value()) {
    std::cout << "Set blocking_paranoia_level error: " << result.error() << std::endl;
    return 1;
  }

  for (auto& rule_file : rule_files) {
    result = engine.loadFromFile(rule_file);
    if (!result.has_value()) {
      std::cout << "Load rules error: " << result.error() << std::endl;
      return 1;
    }
  }

  engine.init();

  // Start benchmark
  std::vector<std::thread> threads;
  Wge::Common::Duration duration;
  for (int i = 0; i < concurrency; ++i) {
    threads.emplace_back(std::thread(thread_func, std::ref(engine), max_test_count,
                                     std::ref(test_data_white), std::ref(test_data_black)));
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
            << "QPS:" << 1000.0 * test_count / duration.milliseconds() << std::endl;

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