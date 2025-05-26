#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

#include "common/duration.h"
#include "modsecurity/modsecurity.h"
#include "modsecurity/rule_message.h"
#include "modsecurity/rules_set.h"
#include "modsecurity/transaction.h"

#include "../test_data/test_data.h"

uint32_t test_count = 0;
std::mutex mutex;

void process(modsecurity::ModSecurity& engine, modsecurity::RulesSet& rules_set,
             const HttpInfo& http_info) {
  modsecurity::Transaction t(&engine, &rules_set, nullptr);

  t.processConnection("192.168.1.100", 20000, "192.168.1.200", 80);
  t.processURI(
      std::string(http_info.request_uri_.data(), http_info.request_uri_.size()).c_str(),
      std::string(http_info.request_method_.data(), http_info.request_method_.size()).c_str(),
      std::string(http_info.request_version_.data(), http_info.request_version_.size()).c_str());

  for (auto& [key, value] : http_info.request_headers_) {
    t.addRequestHeader({key.data(), key.length()}, {value.data(), value.length()});
  }
  t.processRequestHeaders();
  t.appendRequestBody(reinterpret_cast<const unsigned char*>(http_info.request_body_.data()), http_info.request_body_.length());
  t.processRequestBody();

  for (auto& [key, value] : http_info.response_headers_) {
    t.addResponseHeader({key.data(), key.length()}, {value.data(), value.length()});
  }
  t.processResponseHeaders(200, "HTTP/1.1");
  t.appendResponseBody(reinterpret_cast<const unsigned char*>(http_info.response_body_.data()), http_info.response_body_.length());
  t.processResponseBody();
}

void logCb(void* data, const void* message) {
  const modsecurity::RuleMessage* rule_message =
      reinterpret_cast<const modsecurity::RuleMessage*>(message);
  // std::cout << rule_message->m_rule.m_ruleId << std::endl;
}

void thread_func(modsecurity::ModSecurity& engine, modsecurity::RulesSet& rules_set,
                 uint32_t max_test_count, const TestData& test_data_white,
                 const TestData& test_data_black) {
  while (true) {
    auto& white_data = test_data_white.getHttpInfos();
    auto& black_data = test_data_black.getHttpInfos();

    for (auto& http_info : white_data) {
      process(engine, rules_set, http_info);
    }
    for (auto& http_info : black_data) {
      process(engine, rules_set, http_info);
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
  TestData test_data_black(TestData::Type::Black);

  // Load rules
  modsecurity::ModSecurity engine;
  modsecurity::RulesSet rules_set;
  engine.setServerLogCb(logCb, modsecurity::RuleMessageLogProperty |
                                   modsecurity::IncludeFullHighlightLogProperty);
  int result;
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
  result = rules_set.load(
      R"(SecAction "id:205, phase:1,nolog,pass,t:none,setvar:tx.blocking_paranoia_level=4")");
  if (result == -1) {
    std::cout << "Set blocking_paranoia_level error: " << rules_set.getParserError() << std::endl;
    return 1;
  }

  for (auto& rule_file : rule_files) {
    result = rules_set.loadFromUri(rule_file.c_str());
    if (result == -1) {
      std::cout << "Load rules error: " << rules_set.getParserError() << std::endl;
      return 1;
    }
  }

  // Start benchmark
  std::vector<std::thread> threads;
  Wge::Common::Duration duration;
  for (int i = 0; i < concurrency; ++i) {
    threads.emplace_back(std::thread(thread_func, std::ref(engine), std::ref(rules_set),
                                     max_test_count, std::ref(test_data_white),
                                     std::ref(test_data_black)));
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