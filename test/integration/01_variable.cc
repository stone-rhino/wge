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
#include <gtest/gtest.h>

#include "antlr4/parser.h"
#include "engine.h"
#include "variable/variables_include.h"

namespace Wge {
namespace Integration {
class VariableTest : public testing::Test {
public:
  VariableTest() : engine_(spdlog::level::off) {}

public:
  void SetUp() override {
    request_header_find_ = [&](const std::string& key) {
      std::vector<std::string_view> result;
      auto range = request_headers_.equal_range(key);
      for (auto iter = range.first; iter != range.second; ++iter) {
        result.emplace_back(iter->second.data(), iter->second.length());
      }

      return result;
    };

    request_header_traversal_ = [&](HeaderTraversalCallback callback) {
      for (auto& [key, value] : request_headers_) {
        if (!callback(key, value)) {
          break;
        }
      }
    };

    engine_.init();
    t_ = engine_.makeTransaction();
    t_->processConnection(downstream_ip_, downstream_port_, upstream_ip_, upstream_port_);
    t_->processUri(uri_, method_, version_);
    t_->processRequestHeaders(request_header_find_, request_header_traversal_,
                              request_headers_.size(), nullptr);
  }

protected:
  Engine engine_;
  TransactionPtr t_;
  HeaderFind request_header_find_;
  HeaderTraversal request_header_traversal_;
  HeaderFind response_header_find_;
  HeaderTraversal response_header_traversal_;

protected:
  std::string downstream_ip_{"192.168.1.100"};
  short downstream_port_{20000};
  std::string upstream_ip_{"192.168.1.200"};
  short upstream_port_{80};

  std::string uri_{"/?p1=v1&p2=v2&p3=v3&p4=v4"};
  std::string method_{"GET"};
  std::string version_{"1.1"};

  std::unordered_multimap<std::string, std::string> request_headers_{
      {"host", "localhost:80"},
      {"user-agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like "
                     "Gecko) Chrome/124.0.0.0 Safari/537.36"},
      {"accept", "text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/"
                 "webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7"},
      {"x-forwarded-proto", "http"},
      {"cookie", "aa=bb"},
      {"cookie", "cc=dd"}};
};

TEST_F(VariableTest, ARGS_COMBINED_SIZE) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, ARGS_GET_NAMES) {
  Common::EvaluateResults result;

  Variable::ArgsGetNames all("", false, false, "");
  result.clear();
  all.evaluate(*t_, result);
  EXPECT_EQ(result.size(), 4);

  Variable::ArgsGetNames all_count("", false, true, "");
  result.clear();
  all_count.evaluate(*t_, result);
  EXPECT_EQ(result.size(), 1);
  EXPECT_EQ(std::get<int64_t>(result.front().variant_), 4);

  Variable::ArgsGetNames sub("p1", false, false, "");
  result.clear();
  sub.evaluate(*t_, result);
  EXPECT_EQ(result.size(), 1);
  EXPECT_EQ(std::get<std::string_view>(result.front().variant_), "p1");

  Variable::ArgsGetNames sub_count("p1", false, true, "");
  result.clear();
  sub_count.evaluate(*t_, result);
  EXPECT_EQ(result.size(), 1);
  EXPECT_EQ(std::get<int64_t>(result.front().variant_), 1);

  {
    Variable::ArgsGetNames sub_regex("/^p/", false, false, "");
    result.clear();
    sub_regex.evaluate(*t_, result);
    EXPECT_EQ(result.size(), 4);
  }

  {
    Variable::ArgsGetNames sub_regex("/^p\\d/", false, false, "");
    result.clear();
    sub_regex.evaluate(*t_, result);
    EXPECT_EQ(result.size(), 4);
  }

  {
    Variable::ArgsGetNames sub_regex("/^pa/", false, false, "");
    result.clear();
    sub_regex.evaluate(*t_, result);
    EXPECT_EQ(result.size(), 0);
  }

  {
    Variable::ArgsGetNames sub_regex("@test/integration/01_variable_test.data@", false, false, "");
    result.clear();
    sub_regex.evaluate(*t_, result);
    EXPECT_EQ(result.size(), 2);
  }
}

TEST_F(VariableTest, ARGS_GET) {
  Common::EvaluateResults result;

  Variable::ArgsGet all("", false, false, "");
  result.clear();
  all.evaluate(*t_, result);
  EXPECT_EQ(result.size(), 4);

  Variable::ArgsGet all_count("", false, true, "");
  result.clear();
  all_count.evaluate(*t_, result);
  EXPECT_EQ(result.size(), 1);
  EXPECT_EQ(std::get<int64_t>(result.front().variant_), 4);

  Variable::ArgsGet sub("p1", false, false, "");
  result.clear();
  sub.evaluate(*t_, result);
  EXPECT_EQ(result.size(), 1);
  EXPECT_EQ(std::get<std::string_view>(result.front().variant_), "v1");

  Variable::ArgsGet sub_count("p1", false, true, "");
  result.clear();
  sub_count.evaluate(*t_, result);
  EXPECT_EQ(result.size(), 1);
  EXPECT_EQ(std::get<int64_t>(result.front().variant_), 1);

  {
    Variable::ArgsGet sub_regex("/^p/", false, false, "");
    result.clear();
    sub_regex.evaluate(*t_, result);
    EXPECT_EQ(result.size(), 4);
  }

  {
    Variable::ArgsGet sub_regex("/^p\\d/", false, false, "");
    result.clear();
    sub_regex.evaluate(*t_, result);
    EXPECT_EQ(result.size(), 4);
  }

  {
    Variable::ArgsGet sub_regex("/^pa/", false, false, "");
    result.clear();
    sub_regex.evaluate(*t_, result);
    EXPECT_EQ(result.size(), 0);
  }

  {
    Variable::ArgsGet sub_regex("@test/integration/01_variable_test.data@", false, false, "");
    result.clear();
    sub_regex.evaluate(*t_, result);
    EXPECT_EQ(result.size(), 2);
  }
}

TEST_F(VariableTest, ARGS_NAMES) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, ARGS_POST_NAMES) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, ARGS_POST) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, ARGS) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, AUTH_TYPE) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, DURATION) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, ENV) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, FILES_COMBINED_SIZE) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, FILES_NAMES) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, FILES_SIZES) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, FILES_TMPNAMES) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, FILES_TMP_CONTENT) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, FILES) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, FULL_REQUEST_LENGTH) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, FULL_REQUEST) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, GEO) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, HIGHEST_SEVERITY) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, INBOUND_DATA_ERROR) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, MATCHED_VAR_NAME) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, MATCHED_VAR) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, MATCHED_VARS_NAMES) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, MATCHED_VARS) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, MODSEC_BUILD) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, MSC_PCRE_LIMITS_EXCEEDED) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, MULTIPART_BOUNDARY_QUOTED) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, MULTIPART_BOUNDARY_WHITESPACE) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, MULTIPART_CRLF_LF_LINES) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, MULTIPART_DATA_AFTER) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, MULTIPART_DATA_BEFORE) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, MULTIPART_FILE_LIMIT_EXCEEDED) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, MULTIPART_FILENAME) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, MULTIPART_HEADER_FOLDING) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, MULTIPART_INVALID_HEADER_FOLDING) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, MULTIPART_INVALID_PART) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, MULTIPART_INVALID_QUOTING) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, MULTIPART_LF_LINE) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, MULTIPART_MISSING_SEMICOLON) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, MULTIPART_NAME) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, MULTIPART_PART_HEADERS) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, MULTIPART_STRICT_ERROR) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, MULTIPART_UNMATCHED_BOUNDARY) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, OUTBOUND_DATA_ERROR) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, PATH_INFO) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, QUERY_STRING) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, REMOTE_ADDR) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, REMOTE_HOST) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, REMOTE_PORT) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, REMOTE_USER) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, REQBODY_ERROR_MSG) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, REQBODY_ERROR) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, REQBODY_PROCESSOR_ERROR) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, REQBODY_PROCESSOR) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, REQUEST_BASENAME) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, REQUEST_BODY_LENGTH) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, REQUEST_BODY) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, REQUEST_COOKIES_NAMES) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, REQUEST_COOKIES) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, REQUEST_FILENAME) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, REQUEST_HEADERS_NAMES) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, REQUEST_HEADERS) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, REQUEST_LINE) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, REQUEST_METHOD) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, REQUEST_PROTOCOL) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, REQUEST_URI_RAW) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, REQUEST_URI) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, RESPONSE_BODY) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, RESPONSE_CONTENT_LENGTH) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, RESPONSE_CONTENT_TYPE) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, RESPONSE_HEADERS_NAMES) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, RESPONSE_HEADERS) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, RESPONSE_PROTOCOL) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, RESPONSE_STATUS) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, RULE) {
  const std::string directive =
      R"(SecRuleEngine On
      SecRule RULE:id|RULE:phase "@eq 1" "id:1,phase:1,setvar:'tx.test_count=+1',setvar:'tx.operator_value=%{RULE.operator_value}'")";

  Engine engine;
  auto result = engine.load(directive);
  engine.init();
  auto t = engine.makeTransaction();
  ASSERT_TRUE(result.has_value());

  t->processRequestHeaders(nullptr, nullptr, 0, nullptr);
  ASSERT_TRUE(t->hasVariable("", "test_count"));
  EXPECT_EQ(std::get<int64_t>(t->getVariable("", "test_count")), 2);
  ASSERT_TRUE(t->hasVariable("", "operator_value"));
  EXPECT_EQ(std::get<std::string_view>(t->getVariable("", "operator_value")), "1");
}

TEST_F(VariableTest, SERVER_ADDR) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, SERVER_NAME) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, SERVER_PORT) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, SESSION) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, SESSIONID) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, STATUS_LINE) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, TIME_DAY) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, TIME_EPOCH) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, TIME_HOUR) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, TIME_MIN) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, TIME_MON) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, TIME_SEC) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, TIME_WDAY) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, TIME_YEAR) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, TIME) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, TX) {
  Common::EvaluateResults result;

  t_->setVariable("", "global_foo", "global_foo_value");
  t_->setVariable("", "global_bar", "global_bar_value");
  t_->setVariable("ns", "ns_foo", "ns_foo_value");
  t_->setVariable("ns", "ns_bar", "ns_bar_value");
  t_->setVariable("ns", "ns_foo1", "ns_foo1_value");
  t_->setVariable("ns", "ns_bar1", "ns_bar1_value");

  Variable::Tx global_all("", "", std::nullopt, false, false, "");
  result.clear();
  global_all.evaluate(*t_, result);
  EXPECT_EQ(result.size(), 2);
  EXPECT_EQ(std::get<std::string_view>(result[0].variant_), "global_foo_value");
  EXPECT_EQ(std::get<std::string_view>(result[1].variant_), "global_bar_value");

  Variable::Tx global_all_count("", "", std::nullopt, false, true, "");
  result.clear();
  global_all_count.evaluate(*t_, result);
  EXPECT_EQ(result.size(), 1);
  EXPECT_EQ(std::get<int64_t>(result[0].variant_), 2);

  Variable::Tx global_sub("", "global_foo", std::nullopt, false, false, "");
  result.clear();
  global_sub.evaluate(*t_, result);
  EXPECT_EQ(result.size(), 1);
  EXPECT_EQ(std::get<std::string_view>(result[0].variant_), "global_foo_value");

  Variable::Tx global_sub_count("", "global_foo", std::nullopt, false, true, "");
  result.clear();
  global_sub_count.evaluate(*t_, result);
  EXPECT_EQ(result.size(), 1);
  EXPECT_EQ(std::get<int64_t>(result[0].variant_), 1);

  {
    Variable::Tx global_sub_regex("", "/^global_/", std::nullopt, false, false, "");
    result.clear();
    global_sub_regex.evaluate(*t_, result);
    EXPECT_EQ(result.size(), 2);
    EXPECT_EQ(std::get<std::string_view>(result[0].variant_), "global_foo_value");
    EXPECT_EQ(std::get<std::string_view>(result[1].variant_), "global_bar_value");
  }

  {
    Variable::Tx global_sub_regex("", "/^global_f/", std::nullopt, false, false, "");
    result.clear();
    global_sub_regex.evaluate(*t_, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<std::string_view>(result[0].variant_), "global_foo_value");
  }

  {
    Variable::Tx global_sub_regex("", "/^global_b/", std::nullopt, false, false, "");
    result.clear();
    global_sub_regex.evaluate(*t_, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<std::string_view>(result[0].variant_), "global_bar_value");
  }

  Variable::Tx ns_all("ns", "", std::nullopt, false, false, "");
  result.clear();
  ns_all.evaluate(*t_, result);
  EXPECT_EQ(result.size(), 4);
  EXPECT_EQ(std::get<std::string_view>(result[0].variant_), "ns_foo_value");
  EXPECT_EQ(std::get<std::string_view>(result[1].variant_), "ns_bar_value");
  EXPECT_EQ(std::get<std::string_view>(result[2].variant_), "ns_foo1_value");
  EXPECT_EQ(std::get<std::string_view>(result[3].variant_), "ns_bar1_value");

  Variable::Tx ns_all_count("ns", "", std::nullopt, false, true, "");
  result.clear();
  ns_all_count.evaluate(*t_, result);
  EXPECT_EQ(result.size(), 1);
  EXPECT_EQ(std::get<int64_t>(result[0].variant_), 4);

  Variable::Tx ns_sub("ns", "ns_foo", std::nullopt, false, false, "");
  result.clear();
  ns_sub.evaluate(*t_, result);
  EXPECT_EQ(result.size(), 1);
  EXPECT_EQ(std::get<std::string_view>(result[0].variant_), "ns_foo_value");

  Variable::Tx ns_sub_count("ns", "ns_foo", std::nullopt, false, true, "");
  result.clear();
  ns_sub_count.evaluate(*t_, result);
  EXPECT_EQ(result.size(), 1);
  EXPECT_EQ(std::get<int64_t>(result[0].variant_), 1);

  {
    Variable::Tx ns_sub_regex("ns", "/^ns_fo/", std::nullopt, false, false, "");
    result.clear();
    ns_sub_regex.evaluate(*t_, result);
    EXPECT_EQ(result.size(), 2);
    EXPECT_EQ(std::get<std::string_view>(result[0].variant_), "ns_foo_value");
    EXPECT_EQ(std::get<std::string_view>(result[1].variant_), "ns_foo1_value");
  }

  {
    Variable::Tx ns_sub_regex("ns", "/^ns_ba/", std::nullopt, false, false, "");
    result.clear();
    ns_sub_regex.evaluate(*t_, result);
    EXPECT_EQ(result.size(), 2);
    EXPECT_EQ(std::get<std::string_view>(result[0].variant_), "ns_bar_value");
    EXPECT_EQ(std::get<std::string_view>(result[1].variant_), "ns_bar1_value");
  }
}

TEST_F(VariableTest, UNIQUE_ID) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, URLENCODED_ERROR) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, USERID) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, WEBAPPID) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, XML) {
  std::string_view xml_body =
      R"(<bookstore><book id="1" category="fiction"><title lang="en">XML Guide</title><author>John Doe</author></book></bookstore>)";

  const std::string directive = R"(
        SecRuleEngine On
        SecAction "id:100,phase:1,ctl:requestBodyProcessor=XML"
        SecRule XML:/* "@streq XML GuideJohn Doe" \
          "id:1, \
          phase: 2, \
          setvar:tx.tag_values_str"
        SecRule XML://@* "@unconditionalMatch" \
          "id:2, \
          phase: 2, \
          setvar:tx.tag_values_str_count=+1, \
          setvar:tx.tag_attr_str_%{tx.tag_values_str_count}=%{MATCHED_VAR}"
        SecRule XML:/*@test/integration/01_variable_test.data@ "@unconditionalMatch" \
          "id:3, \
          phase: 2, \
          setvar:tx.tag_value_pmf=%{MATCHED_VAR}"
        SecRule XML://@*@test/integration/01_variable_test.data@ "@unconditionalMatch" \
          "id:4, \
          phase: 2, \
          setvar:tx.tag_attr_value_pmf=%{MATCHED_VAR}")";

  Engine engine(spdlog::level::off);
  auto result = engine.load(directive);
  engine.init();
  auto t = engine.makeTransaction();
  ASSERT_TRUE(result.has_value());

  t->processRequestHeaders(request_header_find_, request_header_traversal_, request_headers_.size(),
                           nullptr);
  t->processRequestBody(xml_body);

  // rule id: 1
  EXPECT_TRUE(t->hasVariable("", "tag_values_str"));

  // rule id: 2
  EXPECT_EQ(std::get<int64_t>(t->getVariable("", "tag_values_str_count")), 3);
  EXPECT_EQ(std::get<std::string_view>(t->getVariable("", "tag_attr_str_1")), "1");
  EXPECT_EQ(std::get<std::string_view>(t->getVariable("", "tag_attr_str_2")), "fiction");
  EXPECT_EQ(std::get<std::string_view>(t->getVariable("", "tag_attr_str_3")), "en");

  // rule id: 3
  EXPECT_EQ(std::get<std::string_view>(t->getVariable("", "tag_value_pmf")), "XML Guide");

  // rule id: 4
  EXPECT_EQ(std::get<std::string_view>(t->getVariable("", "tag_attr_value_pmf")), "en");

  // Test for parse xml into args option(On)
  {
    const std::string directive = R"(
        SecRuleEngine On
        SecAction "id:100,phase:1,ctl:requestBodyProcessor=XML"
        SecParseXmlIntoArgs On
        SecRule XML:/* "@streq XML GuideJohn Doe" \
          "id:1, \
          phase: 2, \
          setvar:tx.tag_values_str"
        SecRule &ARGS "@eq 2" \
          "id:2, \
          phase: 2, \
          setvar: tx.args_count=2")";

    Engine engine(spdlog::level::off);
    auto result = engine.load(directive);
    engine.init();
    auto t = engine.makeTransaction();
    ASSERT_TRUE(result.has_value());

    t->processRequestHeaders(request_header_find_, request_header_traversal_,
                             request_headers_.size(), nullptr);
    t->processRequestBody(xml_body);

    // rule id: 1
    EXPECT_TRUE(t->hasVariable("", "tag_values_str"));

    // rule id: 2
    EXPECT_TRUE(t->hasVariable("", "args_count"));
  }

  // Test for parse xml into args option(OnlyArgs)
  {
    const std::string directive = R"(
        SecRuleEngine On
        SecAction "id:100,phase:1,ctl:requestBodyProcessor=XML"
        SecParseXmlIntoArgs OnlyArgs
        SecRule XML:/* "@streq XML GuideJohn Doe" \
          "id:1, \
          phase: 2, \
          setvar:tx.tag_values_str"
        SecRule &ARGS "@eq 2" \
          "id:2, \
          phase: 2, \
          setvar: tx.args_count=2")";

    Engine engine(spdlog::level::off);
    auto result = engine.load(directive);
    engine.init();
    auto t = engine.makeTransaction();
    ASSERT_TRUE(result.has_value());

    t->processRequestHeaders(request_header_find_, request_header_traversal_,
                             request_headers_.size(), nullptr);
    t->processRequestBody(xml_body);

    // rule id: 1
    EXPECT_FALSE(t->hasVariable("", "tag_values_str"));

    // rule id: 2
    EXPECT_TRUE(t->hasVariable("", "args_count"));
  }
}

TEST_F(VariableTest, PTREE) {
  std::string json = R"(
{
    "config": {
        "max_connection": 100,
        "server_list": [
            {
                "host": "192.168.1.1",
                "port": 8080,
                "domain": {
                    "name": "server1.example.com",
                    "expire_time": "2025-12-31"
                },
                "tags": [
                    "production",
                    "v1.0"
                ]
            },
            {
                "host": "192.168.1.2",
                "port": 8081,
                "domain": {
                    "name": "server2.example.com",
                    "expire_time": "2025-12-31"
                },
                "tags": [
                    "staging",
                    "v1.1"
                ]
            }
        ]
    }
})";

  Engine engine(spdlog::level::off);
  auto pt_result = engine.propertyTree(json);
  ASSERT_TRUE(pt_result.has_value());
  engine.init();
  auto t = engine.makeTransaction();
  Common::EvaluateResults result;

  {
    Variable::PTree var("config.max_connection", false, false, "");
    result.clear();
    var.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result[0].variant_), 100);
  }

  {
    Variable::PTree var("config.server_list[].host", false, false, "");
    result.clear();
    var.evaluate(*t, result);
    EXPECT_EQ(result.size(), 2);
    EXPECT_EQ(std::get<std::string_view>(result[0].variant_), "192.168.1.1");
    EXPECT_EQ(std::get<std::string_view>(result[1].variant_), "192.168.1.2");
  }

  {
    Variable::PTree var("config.server_list[].port", false, false, "");
    result.clear();
    var.evaluate(*t, result);
    EXPECT_EQ(result.size(), 2);
    EXPECT_EQ(std::get<int64_t>(result[0].variant_), 8080);
    EXPECT_EQ(std::get<int64_t>(result[1].variant_), 8081);
  }

  {
    Variable::PTree var("config.server_list[].domain{}", false, false, "");
    result.clear();
    var.evaluate(*t, result);
    EXPECT_EQ(result.size(), 4);
    EXPECT_EQ(std::get<std::string_view>(result[0].variant_), "server1.example.com");
    EXPECT_EQ(result[0].variable_sub_name_, "name");
    EXPECT_EQ(std::get<std::string_view>(result[1].variant_), "2025-12-31");
    EXPECT_EQ(result[1].variable_sub_name_, "expire_time");
    EXPECT_EQ(std::get<std::string_view>(result[2].variant_), "server2.example.com");
    EXPECT_EQ(result[2].variable_sub_name_, "name");
    EXPECT_EQ(std::get<std::string_view>(result[3].variant_), "2025-12-31");
    EXPECT_EQ(result[3].variable_sub_name_, "expire_time");
  }

  {
    Variable::PTree var("config.server_list[].tags[]", false, false, "");
    result.clear();
    var.evaluate(*t, result);
    EXPECT_EQ(result.size(), 4);
    EXPECT_EQ(std::get<std::string_view>(result[0].variant_), "production");
    EXPECT_EQ(std::get<std::string_view>(result[1].variant_), "v1.0");
    EXPECT_EQ(std::get<std::string_view>(result[2].variant_), "staging");
    EXPECT_EQ(std::get<std::string_view>(result[3].variant_), "v1.1");
  }
}

TEST_F(VariableTest, MATCHED_OPTREE) {
  std::string json = R"(
{
    "config": {
        "max_connection": 100,
        "server_list": [
            {
                "host": "192.168.1.1",
                "port": 8080,
                "domain": {
                    "name": "server1.example.com",
                    "expire_time": "2025-12-31"
                },
                "tags": [
                    "production",
                    "v1.0"
                ]
            },
            {
                "host": "192.168.1.2",
                "port": 8081,
                "domain": {
                    "name": "server2.example.com",
                    "expire_time": "2025-12-31"
                },
                "tags": [
                    "staging",
                    "v1.1"
                ]
            }
        ]
    }
})";

  Engine engine(spdlog::level::off);
  auto pt_result = engine.propertyTree(json);
  ASSERT_TRUE(pt_result.has_value());
  engine.init();
  auto t = engine.makeTransaction();
  Common::EvaluateResults result;

  // config.server_list[].port../host
  {
    auto& matched_optree =
        engine.propertyTree().get_child("config.server_list").front().second.get_child("port");
    EXPECT_EQ(std::get<int64_t>(matched_optree.data()), 8080);
    t->stageMatchedOPTree(static_cast<const Common::PropertyTree*>(&matched_optree));
    t->commitMatchedOPTree(-1);
    Variable::MatchedOPTree var("../host", false, false, "");
    result.clear();
    var.evaluate(*t, result);
    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<std::string_view>(result[0].variant_), "192.168.1.1");
  }

  // config.server_list[].tags[]../../domain.name
  {
    auto& matched_optree = engine.propertyTree()
                               .get_child("config.server_list")
                               .back()
                               .second.get_child("tags")
                               .front()
                               .second;
    EXPECT_EQ(std::get<std::string_view>(matched_optree.data()), "staging");
    t->stageMatchedOPTree(static_cast<const Common::PropertyTree*>(&matched_optree));
    t->commitMatchedOPTree(-1);
    Variable::MatchedOPTree var("../../domain.name", false, false, "");
    result.clear();
    var.evaluate(*t, result);
    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<std::string_view>(result[0].variant_), "server2.example.com");
  }
}

TEST_F(VariableTest, MATCHED_VPTREE) {
  std::string json = R"(
{
    "config": {
        "max_connection": 100,
        "server_list": [
            {
                "host": "192.168.1.1",
                "port": 8080,
                "domain": {
                    "name": "server1.example.com",
                    "expire_time": "2025-12-31"
                },
                "tags": [
                    "production",
                    "v1.0"
                ]
            },
            {
                "host": "192.168.1.2",
                "port": 8081,
                "domain": {
                    "name": "server2.example.com",
                    "expire_time": "2025-12-31"
                },
                "tags": [
                    "staging",
                    "v1.1"
                ]
            }
        ]
    }
})";

  Engine engine(spdlog::level::off);
  auto pt_result = engine.propertyTree(json);
  ASSERT_TRUE(pt_result.has_value());
  engine.init();
  auto t = engine.makeTransaction();
  Common::EvaluateResults result;

  // config.server_list[].port../host
  {
    auto& matched_vptree =
        engine.propertyTree().get_child("config.server_list").front().second.get_child("port");
    EXPECT_EQ(std::get<int64_t>(matched_vptree.data()), 8080);
    t->pushMatchedVPTree(-1, static_cast<const Common::PropertyTree*>(&matched_vptree));
    Variable::MatchedVPTree var("../host", false, false, "");
    result.clear();
    var.evaluate(*t, result);
    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<std::string_view>(result[0].variant_), "192.168.1.1");
  }

  // config.server_list[].tags[]../../domain.name
  {
    auto& matched_vptree = engine.propertyTree()
                               .get_child("config.server_list")
                               .back()
                               .second.get_child("tags")
                               .front()
                               .second;
    EXPECT_EQ(std::get<std::string_view>(matched_vptree.data()), "staging");
    t->pushMatchedVPTree(-1, static_cast<const Common::PropertyTree*>(&matched_vptree));
    Variable::MatchedVPTree var("../../domain.name", false, false, "");
    result.clear();
    var.evaluate(*t, result);
    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<std::string_view>(result[0].variant_), "server2.example.com");
  }
}
} // namespace Integration
} // namespace Wge