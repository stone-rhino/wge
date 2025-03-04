#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

#include "common/duration.h"
#include "modsecurity/modsecurity.h"
#include "modsecurity/rule_message.h"
#include "modsecurity/rules_set.h"
#include "modsecurity/transaction.h"

#include "../test_data/request.h"

uint32_t test_count = 0;
std::mutex mutex;

void logCb(void* data, const void* message) {
  const modsecurity::RuleMessage* rule_message =
      reinterpret_cast<const modsecurity::RuleMessage*>(message);
  std::cout << rule_message->m_rule.m_ruleId << std::endl;
}

void thread_func(modsecurity::ModSecurity& engine, modsecurity::RulesSet& rules_set,
                 uint32_t max_test_count) {
  while (true) {
    {
      std::lock_guard<std::mutex> lock(mutex);
      if (test_count >= max_test_count) {
        break;
      }
      ++test_count;
    }

    Request request;
    modsecurity::Transaction t(&engine, &rules_set, nullptr);

    t.processConnection(request.downstream_ip_.c_str(), request.downstream_port_,
                        request.upstream_ip_.c_str(), request.upstream_port_);
    t.processURI(request.path_.c_str(), request.protocol_.c_str(), request.version_.c_str());

    for (auto& [key, value] : request.request_headers_) {
      t.addRequestHeader(key, value);
    }
    t.processRequestHeaders();
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
  modsecurity::ModSecurity engine;
  modsecurity::RulesSet rules_set;
  engine.setServerLogCb(logCb, modsecurity::RuleMessageLogProperty |
                                   modsecurity::IncludeFullHighlightLogProperty);
  int result;
  std::vector<std::string> rule_files = {
      "test/test_data/waf-conf/base/engin-setup.conf",
      "test/test_data/waf-conf/base/crs-setup.conf",
      "test/test_data/waf-conf/coreruleset/rules/REQUEST-901-INITIALIZATION.conf",
      // "test/test_data/waf-conf/coreruleset/rules/REQUEST-905-COMMON-EXCEPTIONS.conf",
      // "test/test_data/waf-conf/coreruleset/rules/REQUEST-911-METHOD-ENFORCEMENT.conf",
      // "test/test_data/waf-conf/coreruleset/rules/REQUEST-913-SCANNER-DETECTION.conf",
      // "test/test_data/waf-conf/coreruleset/rules/REQUEST-920-PROTOCOL-ENFORCEMENT.conf",
      // "test/test_data/waf-conf/coreruleset/rules/REQUEST-921-PROTOCOL-ATTACK.conf",
      // "test/test_data/waf-conf/coreruleset/rules/REQUEST-922-MULTIPART-ATTACK.conf",
      // "test/test_data/waf-conf/coreruleset/rules/REQUEST-930-APPLICATION-ATTACK-LFI.conf",
      // "test/test_data/waf-conf/coreruleset/rules/REQUEST-931-APPLICATION-ATTACK-RFI.conf",
      // "test/test_data/waf-conf/coreruleset/rules/REQUEST-932-APPLICATION-ATTACK-RCE.conf",
      // "test/test_data/waf-conf/coreruleset/rules/REQUEST-933-APPLICATION-ATTACK-PHP.conf",
      // "test/test_data/waf-conf/coreruleset/rules/REQUEST-934-APPLICATION-ATTACK-GENERIC.conf",
      // "test/test_data/waf-conf/coreruleset/rules/REQUEST-941-APPLICATION-ATTACK-XSS.conf",
      // "test/test_data/waf-conf/coreruleset/rules/REQUEST-942-APPLICATION-ATTACK-SQLI.conf",
      // "test/test_data/waf-conf/coreruleset/rules/"
      // "REQUEST-943-APPLICATION-ATTACK-SESSION-FIXATION.conf",
      // "test/test_data/waf-conf/coreruleset/rules/REQUEST-944-APPLICATION-ATTACK-JAVA.conf",
      // "test/test_data/waf-conf/coreruleset/rules/REQUEST-949-BLOCKING-EVALUATION.conf",
      // "test/test_data/waf-conf/coreruleset/rules/RESPONSE-950-DATA-LEAKAGES.conf",
      // "test/test_data/waf-conf/coreruleset/rules/RESPONSE-951-DATA-LEAKAGES-SQL.conf",
      // "test/test_data/waf-conf/coreruleset/rules/RESPONSE-952-DATA-LEAKAGES-JAVA.conf",
      // "test/test_data/waf-conf/coreruleset/rules/RESPONSE-953-DATA-LEAKAGES-PHP.conf",
      // "test/test_data/waf-conf/coreruleset/rules/RESPONSE-954-DATA-LEAKAGES-IIS.conf",
      // "test/test_data/waf-conf/coreruleset/rules/RESPONSE-955-DATA-LEAKAGES-APACHE.conf",
      // "test/test_data/waf-conf/coreruleset/rules/RESPONSE-959-BLOCKING-EVALUATION.conf",
      // "test/test_data/waf-conf/coreruleset/rules/RESPONSE-980-CORRELATION.conf",
  };
  for (auto& rule_file : rule_files) {
    result = rules_set.loadFromUri(rule_file.c_str());
    if (result == -1) {
      std::cout << "Load rules error: " << rules_set.getParserError() << std::endl;
      return 1;
    }
  }

  // Start benchmark
  std::vector<std::thread> threads;
  SrSecurity::Common::Duration duration;
  for (int i = 0; i < concurrency; ++i) {
    threads.emplace_back(
        std::thread(thread_func, std::ref(engine), std::ref(rules_set), max_test_count));
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