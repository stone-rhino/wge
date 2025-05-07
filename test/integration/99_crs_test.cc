/**
 * Copyright (c) 2024-2025 Stone Rhino and contributors.
 *
 * MIT License (http://opensource.org/licenses/MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
 * associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
 * NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#include <future>
#include <unordered_map>

#include <gtest/gtest.h>

#include "engine.h"

namespace Wge {
namespace Integration {
class CrsTest : public testing::Test {
public:
  CrsTest() : engine_(spdlog::level::trace) {}

public:
  void SetUp() override {
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
    result = engine_.load(
        R"(SecAction "id:205, phase:1,nolog,pass,t:none,setvar:tx.blocking_paranoia_level=4")");
    if (!result.has_value()) {
      std::cout << "Set blocking_paranoia_level error: " << result.error() << std::endl;
      return;
    }

    for (auto& rule_file : rule_files) {
      result = engine_.loadFromFile(rule_file);
      if (!result.has_value()) {
        std::cout << "Load rules error: " << result.error() << std::endl;
        return;
      }
    }

    engine_.init();

    request_header_find_ = [&](const std::string& key) {
      std::vector<std::string_view> result;
      auto range = request_headers_.equal_range(key);
      for (auto iter = range.first; iter != range.second; ++iter) {
        result.emplace_back(iter->second.data(), iter->second.length());
      }

      if (result.size() > 0) {
        return result[0];
      } else {
        return std::string_view();
      }
    };

    request_header_traversal_ = [&](HeaderTraversalCallback callback) {
      for (auto& [key, value] : request_headers_) {
        if (!callback(key, value)) {
          break;
        }
      }
    };

    request_body_extractor_ = [&]() -> const std::vector<std::string_view>& {
      return request_body_;
    };

    response_header_find_ = [&](const std::string& key) {
      std::vector<std::string_view> result;
      auto range = response_headers_.equal_range(key);
      for (auto iter = range.first; iter != range.second; ++iter) {
        result.emplace_back(iter->second.data(), iter->second.length());
      }

      if (result.size() > 0) {
        return result[0];
      } else {
        return std::string_view();
      }
    };

    response_header_traversal_ = [&](HeaderTraversalCallback callback) {
      for (auto& [key, value] : response_headers_) {
        if (!callback(key, value)) {
          break;
        }
      }
    };

    response_body_extractor_ = [&]() -> const std::vector<std::string_view>& {
      return response_body_;
    };
  }

protected:
  Engine engine_;
  HeaderFind request_header_find_;
  HeaderTraversal request_header_traversal_;
  BodyExtractor request_body_extractor_;
  HeaderFind response_header_find_;
  HeaderTraversal response_header_traversal_;
  BodyExtractor response_body_extractor_;

protected:
  std::string downstream_ip_{"192.168.1.100"};
  short downstream_port_{20000};
  std::string upstream_ip_{"192.168.1.200"};
  short upstream_port_{80};

  std::string uri_{"/?p1=v1&p2=v2"};
  std::string method_{"GET"};
  std::string version_{"1.1"};

  std::unordered_multimap<std::string, std::string> request_headers_{
      {"host", "localhost:80"},
      {"user-agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like "
                     "Gecko) Chrome/124.0.0.0 Safari/537.36"},
      {"accept", "text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/"
                 "webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7"},
      {"x-forwarded-proto", "http"},
      {"cookie", "c1=v1;c2=v2"},
      {"cookie", "c3=v4"}};

  std::unordered_multimap<std::string, std::string> response_headers_{
      {"content-type", "text/html; charset=UTF-8"},
      {"content-length", "11"},
      {"set-cookie", "c1=v1;c2=v2"},
      {"set-cookie", "c3=v4"}};

  std::vector<std::string_view> request_body_;
  std::vector<std::string_view> response_body_{{"hello world"}};
};

TEST_F(CrsTest, crs) {
  // The test must be run in a separate thread to avoid the thread local scratch space of hyperscan
  // was not correctly clone from the main thread. Because the other test cases may use the
  // hyperscan scanner, and the thread local scratch was initialized by the other test cases in the
  // main thread, so if we run this test in the main thread, the scratch space will be not correctly
  // initialized.
  std::future<void> result = std::async(std::launch::async, [&]() {
    auto t = engine_.makeTransaction();
    t->processConnection(downstream_ip_, downstream_port_, upstream_ip_, upstream_port_);
    t->processUri(uri_, method_, version_);
    t->processRequestHeaders(request_header_find_, request_header_traversal_,
                             request_headers_.size(), nullptr);
    t->processRequestBody(request_body_extractor_, nullptr);
    t->processResponseHeaders("200", "HTTP/1.1", response_header_find_, response_header_traversal_,
                              response_headers_.size(), nullptr);
    t->processResponseBody(response_body_extractor_, nullptr);
  });
  result.get();
}
} // namespace Integration
} // namespace Wge