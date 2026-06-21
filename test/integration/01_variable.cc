/**
 * Copyright (c) 2024-2026 Stone Rhino and contributors.
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

    response_header_find_ = [&](const std::string& key) {
      std::vector<std::string_view> result;
      auto range = response_headers_.equal_range(key);
      for (auto iter = range.first; iter != range.second; ++iter) {
        result.emplace_back(iter->second.data(), iter->second.length());
      }

      return result;
    };

    response_header_traversal_ = [&](HeaderTraversalCallback callback) {
      for (auto& [key, value] : response_headers_) {
        if (!callback(key, value)) {
          break;
        }
      }
    };

    engine_.init();
    engine_.updatePropertyStore(ptree_json_);
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

  std::unordered_multimap<std::string, std::string> response_headers_{
      {"status", "200"},
  };

  std::string ptree_json_{R"(
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
        ],
        "boolean_test1": true,
        "boolean_test2": false,
        "null_test": null,
        "float_test": 3.14159,
        "minus_test": -100,
        "minus_float_test": -3.14159
    }
})"};
};

TEST_F(VariableTest, ARGS_COMBINED_SIZE) {
  Variable::ArgsCombinedSize args_combined_size("", false, false, "");
  Common::EvaluateResults results;
  {
    results.clear();
    args_combined_size.evaluate(*t_, results);
    EXPECT_EQ(std::get<int64_t>(results.front().variant_), 4 * 4);
  }

  {
    const std::string directive = R"(
    SecRuleEngine On
    SecAction "id:100,phase:1,ctl:requestBodyProcessor=URLENCODED"
  )";

    std::string urlencoded_body = "p5=v5&p6=v6&p7=v7";
    Engine engine(spdlog::level::off);
    engine.load(directive);
    engine.init();
    auto t = engine.makeTransaction();
    t->processUri(uri_, "POST", version_);
    t->processRequestHeaders(request_header_find_, request_header_traversal_,
                             request_headers_.size(), nullptr);
    t->processRequestBody(urlencoded_body);
    results.clear();
    args_combined_size.evaluate(*t, results);
    EXPECT_EQ(std::get<int64_t>(results.front().variant_), 4 * 4 + 4 * 3);
  }

  {
    const std::string directive = R"(
    SecRuleEngine On
    SecAction "id:100,phase:1,ctl:requestBodyProcessor=JSON"
  )";

    std::string json_body = R"(
    {
      "p8": "v8",
      "p9": "v9"
    }
    )";
    Engine engine(spdlog::level::off);
    engine.load(directive);
    engine.init();
    auto t = engine.makeTransaction();
    t->processUri(uri_, "POST", version_);
    t->processRequestHeaders(request_header_find_, request_header_traversal_,
                             request_headers_.size(), nullptr);
    t->processRequestBody(json_body);
    results.clear();
    args_combined_size.evaluate(*t, results);
    EXPECT_EQ(std::get<int64_t>(results.front().variant_), 4 * 4 + 4 * 2);
  }
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
  Common::EvaluateResults result;
  {
    const std::string directive = R"(
    SecRuleEngine On
    SecAction "id:100,phase:1,ctl:requestBodyProcessor=URLENCODED"
  )";

    std::string urlencoded_body = "p5=v5&p6=v6&p7=v7";
    request_headers_.emplace("content-type", "application/x-www-form-urlencoded");
    Engine engine(spdlog::level::off);
    engine.load(directive);
    engine.init();
    auto t = engine.makeTransaction();
    t->processUri(uri_, "POST", version_);
    t->processRequestHeaders(request_header_find_, request_header_traversal_,
                             request_headers_.size(), nullptr);
    t->processRequestBody(urlencoded_body);

    Variable::ArgsNames all("", false, false, "");
    result.clear();
    all.evaluate(*t, result);
    EXPECT_EQ(result.size(), 7);

    Variable::ArgsNames all_count("", false, true, "");
    result.clear();
    all_count.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 7);

    Variable::ArgsNames sub("p5", false, false, "");
    result.clear();
    sub.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<std::string_view>(result.front().variant_), "p5");

    Variable::ArgsNames sub_count("p5", false, true, "");
    result.clear();
    sub_count.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 1);

    {
      Variable::ArgsNames sub_regex("/^p/", false, false, "");
      result.clear();
      sub_regex.evaluate(*t, result);
      EXPECT_EQ(result.size(), 7);
    }

    {
      Variable::ArgsNames sub_regex("/^p\\d/", false, false, "");
      result.clear();
      sub_regex.evaluate(*t, result);
      EXPECT_EQ(result.size(), 7);
    }

    {
      Variable::ArgsNames sub_regex("/^pa/", false, false, "");
      result.clear();
      sub_regex.evaluate(*t, result);
      EXPECT_EQ(result.size(), 0);
    }

    {
      Variable::ArgsNames sub_regex("@test/integration/01_variable_test.data@", false, false, "");
      result.clear();
      sub_regex.evaluate(*t, result);
      EXPECT_EQ(result.size(), 2);
    }
    request_headers_.erase("content-type");
  }

  {
    const std::string directive = R"(
    SecRuleEngine On
    SecAction "id:100,phase:1,ctl:requestBodyProcessor=JSON"
  )";

    std::string json_body = R"(
    {
      "p8": "v8",
      "p9": "v9"
    }
    )";
    Engine engine(spdlog::level::off);
    engine.load(directive);
    engine.init();
    auto t = engine.makeTransaction();
    t->processUri(uri_, "POST", version_);
    t->processRequestHeaders(request_header_find_, request_header_traversal_,
                             request_headers_.size(), nullptr);
    t->processRequestBody(json_body);

    Variable::ArgsNames all("", false, false, "");
    result.clear();
    all.evaluate(*t, result);
    EXPECT_EQ(result.size(), 6);

    Variable::ArgsNames all_count("", false, true, "");
    result.clear();
    all_count.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 6);

    Variable::ArgsNames sub("p8", false, false, "");
    result.clear();
    sub.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<std::string_view>(result.front().variant_), "p8");

    Variable::ArgsNames sub_count("p8", false, true, "");
    result.clear();
    sub_count.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 1);

    {
      Variable::ArgsNames sub_regex("/^p/", false, false, "");
      result.clear();
      sub_regex.evaluate(*t, result);
      EXPECT_EQ(result.size(), 6);
    }

    {
      Variable::ArgsNames sub_regex("/^p\\d/", false, false, "");
      result.clear();
      sub_regex.evaluate(*t, result);
      EXPECT_EQ(result.size(), 6);
    }

    {
      Variable::ArgsNames sub_regex("/^pa/", false, false, "");
      result.clear();
      sub_regex.evaluate(*t, result);
      EXPECT_EQ(result.size(), 0);
    }

    {
      Variable::ArgsNames sub_regex("@test/integration/01_variable_test.data@", false, false, "");
      result.clear();
      sub_regex.evaluate(*t, result);
      EXPECT_EQ(result.size(), 2);
    }
  }

  {
    const std::string directive = R"(
    SecRuleEngine On
    SecAction "id:100,phase:1,ctl:requestBodyProcessor=MULTIPART"
  )";

    std::string multipart_body = "----helloworld\r\n"
                                 "content-disposition: form-data; name=foo1\r\n"
                                 "header1: value1\r\n"
                                 "\r\n"
                                 "bar1\r\n"
                                 "----helloworld\r\n"
                                 "content-disposition: form-data; name=foo2\r\n"
                                 "header2: value2\r\n"
                                 "\r\n"
                                 "bar2\r\n"
                                 "----helloworld\r\n"
                                 "content-disposition: form-data; name=foo3\r\n"
                                 "header2: value3\r\n"
                                 "\r\n"
                                 "bar3\r\n"
                                 "----helloworld\r\n"
                                 "content-disposition: form-data; name=file1; filename=hello1\r\n"
                                 "\r\n"
                                 "world\r\n"
                                 "----helloworld\r\n"
                                 "content-disposition: form-data; name=file2; filename=hello2\r\n"
                                 "\r\n"
                                 "world\r\n"
                                 "----helloworld\r\n"
                                 "content-disposition: form-data; name=file3; filename=hello3\r\n"
                                 "\r\n"
                                 "world\r\n"
                                 "----helloworld--";

    request_headers_.emplace("content-type", R"(multipart/form-data; boundary=--helloworld)");
    Engine engine(spdlog::level::off);
    engine.load(directive);
    engine.init();
    auto t = engine.makeTransaction();

    t->processUri(uri_, "POST", version_);
    t->processRequestHeaders(request_header_find_, request_header_traversal_,
                             request_headers_.size(), nullptr);
    t->processRequestBody(multipart_body);

    Variable::ArgsNames all("", false, false, "");
    result.clear();
    all.evaluate(*t, result);
    EXPECT_EQ(result.size(), 7);

    Variable::ArgsNames all_count("", false, true, "");
    result.clear();
    all_count.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 7);

    Variable::ArgsNames sub("foo1", false, false, "");
    result.clear();
    sub.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<std::string_view>(result.front().variant_), "foo1");

    Variable::ArgsNames sub_count("foo1", false, true, "");
    result.clear();
    sub_count.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 1);

    {
      Variable::ArgsNames sub_regex("/^foo/", false, false, "");
      result.clear();
      sub_regex.evaluate(*t, result);
      EXPECT_EQ(result.size(), 3);
    }

    {
      Variable::ArgsNames sub_regex("/^foo\\d/", false, false, "");
      result.clear();
      sub_regex.evaluate(*t, result);
      EXPECT_EQ(result.size(), 3);
    }

    {
      Variable::ArgsNames sub_regex("/^fa/", false, false, "");
      result.clear();
      sub_regex.evaluate(*t, result);
      EXPECT_EQ(result.size(), 0);
    }

    {
      Variable::ArgsNames sub_regex("@test/integration/01_variable_test.data@", false, false, "");
      result.clear();
      sub_regex.evaluate(*t, result);
      EXPECT_EQ(result.size(), 2);
    }

    request_headers_.erase("content-type");
  }

  {
    const std::string directive = R"(
    SecRuleEngine On
    SecAction "id:100,phase:1,ctl:requestBodyProcessor=XML"
    SecParseXmlIntoArgs On
  )";

    std::string_view xml_body =
        R"(<bookstore><book id="1" category="fiction"><title lang="en">XML Guide</title><author>John Doe</author></book></bookstore>)";
    Engine engine(spdlog::level::trace);
    engine.load(directive);
    engine.init();
    auto t = engine.makeTransaction();
    t->processUri(uri_, "POST", version_);
    t->processRequestHeaders(request_header_find_, request_header_traversal_,
                             request_headers_.size(), nullptr);
    t->processRequestBody(xml_body);

    Variable::ArgsNames all("", false, false, "");
    result.clear();
    all.evaluate(*t, result);
    EXPECT_EQ(result.size(), 6);

    Variable::ArgsNames all_count("", false, true, "");
    result.clear();
    all_count.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 6);

    Variable::ArgsNames sub1("title", false, false, "");
    result.clear();
    sub1.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<std::string_view>(result.front().variant_), "title");

    Variable::ArgsNames sub2("author", false, false, "");
    result.clear();
    sub2.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<std::string_view>(result.front().variant_), "author");

    Variable::ArgsNames sub_count("title", false, true, "");
    result.clear();
    sub_count.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 1);

    {
      Variable::ArgsNames sub_regex("/author/", false, false, "");
      result.clear();
      sub_regex.evaluate(*t, result);
      EXPECT_EQ(result.size(), 1);
    }

    {
      Variable::ArgsNames sub_regex("/title/", false, false, "");
      result.clear();
      sub_regex.evaluate(*t, result);
      EXPECT_EQ(result.size(), 1);
    }

    {
      Variable::ArgsNames sub_regex("/^(t|a)/", false, false, "");
      result.clear();
      sub_regex.evaluate(*t, result);
      EXPECT_EQ(result.size(), 2);
    }

    {
      Variable::ArgsNames sub_regex("@test/integration/01_variable_test.data@", false, false, "");
      result.clear();
      sub_regex.evaluate(*t, result);
      EXPECT_EQ(result.size(), 3);
    }
  }
}

TEST_F(VariableTest, ARGS_POST_NAMES) {
  Common::EvaluateResults result;
  {
    const std::string directive = R"(
    SecRuleEngine On
    SecAction "id:100,phase:1,ctl:requestBodyProcessor=URLENCODED"
  )";

    std::string urlencoded_body = "p5=v5&p6=v6&p7=v7";
    request_headers_.emplace("content-type", "application/x-www-form-urlencoded");
    Engine engine(spdlog::level::off);
    engine.load(directive);
    engine.init();
    auto t = engine.makeTransaction();
    t->processUri(uri_, "POST", version_);
    t->processRequestHeaders(request_header_find_, request_header_traversal_,
                             request_headers_.size(), nullptr);
    t->processRequestBody(urlencoded_body);

    Variable::ArgsPostNames all("", false, false, "");
    result.clear();
    all.evaluate(*t, result);
    EXPECT_EQ(result.size(), 3);

    Variable::ArgsPostNames all_count("", false, true, "");
    result.clear();
    all_count.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 3);

    Variable::ArgsPostNames sub("p5", false, false, "");
    result.clear();
    sub.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<std::string_view>(result.front().variant_), "p5");

    Variable::ArgsPostNames sub_count("p5", false, true, "");
    result.clear();
    sub_count.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 1);

    {
      Variable::ArgsPostNames sub_regex("/^p/", false, false, "");
      result.clear();
      sub_regex.evaluate(*t, result);
      EXPECT_EQ(result.size(), 3);
    }

    {
      Variable::ArgsPostNames sub_regex("/^p\\d/", false, false, "");
      result.clear();
      sub_regex.evaluate(*t, result);
      EXPECT_EQ(result.size(), 3);
    }

    {
      Variable::ArgsPostNames sub_regex("/^pa/", false, false, "");
      result.clear();
      sub_regex.evaluate(*t, result);
      EXPECT_EQ(result.size(), 0);
    }

    {
      Variable::ArgsPostNames sub_regex("@test/integration/01_variable_test.data@", false, false,
                                        "");
      result.clear();
      sub_regex.evaluate(*t, result);
      EXPECT_EQ(result.size(), 0);
    }
    request_headers_.erase("content-type");
  }

  {
    const std::string directive = R"(
    SecRuleEngine On
    SecAction "id:100,phase:1,ctl:requestBodyProcessor=MULTIPART"
  )";

    std::string multipart_body = "----helloworld\r\n"
                                 "content-disposition: form-data; name=foo1\r\n"
                                 "header1: value1\r\n"
                                 "\r\n"
                                 "bar1\r\n"
                                 "----helloworld\r\n"
                                 "content-disposition: form-data; name=foo2\r\n"
                                 "header2: value2\r\n"
                                 "\r\n"
                                 "bar2\r\n"
                                 "----helloworld\r\n"
                                 "content-disposition: form-data; name=foo3\r\n"
                                 "header2: value3\r\n"
                                 "\r\n"
                                 "bar3\r\n"
                                 "----helloworld\r\n"
                                 "content-disposition: form-data; name=file1; filename=hello1\r\n"
                                 "\r\n"
                                 "world\r\n"
                                 "----helloworld\r\n"
                                 "content-disposition: form-data; name=file2; filename=hello2\r\n"
                                 "\r\n"
                                 "world\r\n"
                                 "----helloworld\r\n"
                                 "content-disposition: form-data; name=file3; filename=hello3\r\n"
                                 "\r\n"
                                 "world\r\n"
                                 "----helloworld--";

    request_headers_.emplace("content-type", R"(multipart/form-data; boundary=--helloworld)");
    Engine engine(spdlog::level::off);
    engine.load(directive);
    engine.init();
    auto t = engine.makeTransaction();

    t->processUri(uri_, "POST", version_);
    t->processRequestHeaders(request_header_find_, request_header_traversal_,
                             request_headers_.size(), nullptr);
    t->processRequestBody(multipart_body);

    Variable::ArgsPostNames all("", false, false, "");
    result.clear();
    all.evaluate(*t, result);
    EXPECT_EQ(result.size(), 3);

    Variable::ArgsPostNames all_count("", false, true, "");
    result.clear();
    all_count.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 3);

    Variable::ArgsPostNames sub("foo1", false, false, "");
    result.clear();
    sub.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<std::string_view>(result.front().variant_), "foo1");

    Variable::ArgsPostNames sub_count("foo1", false, true, "");
    result.clear();
    sub_count.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 1);

    {
      Variable::ArgsPostNames sub_regex("/^foo/", false, false, "");
      result.clear();
      sub_regex.evaluate(*t, result);
      EXPECT_EQ(result.size(), 3);
    }

    {
      Variable::ArgsPostNames sub_regex("/^foo\\d/", false, false, "");
      result.clear();
      sub_regex.evaluate(*t, result);
      EXPECT_EQ(result.size(), 3);
    }

    {
      Variable::ArgsPostNames sub_regex("/^fa/", false, false, "");
      result.clear();
      sub_regex.evaluate(*t, result);
      EXPECT_EQ(result.size(), 0);
    }

    {
      Variable::ArgsPostNames sub_regex("@test/integration/01_variable_test.data@", false, false,
                                        "");
      result.clear();
      sub_regex.evaluate(*t, result);
      EXPECT_EQ(result.size(), 0);
    }

    request_headers_.erase("content-type");
  }

  {
    const std::string directive = R"(
    SecRuleEngine On
    SecAction "id:100,phase:1,ctl:requestBodyProcessor=XML"
    SecParseXmlIntoArgs On
  )";

    std::string_view xml_body =
        R"(<bookstore><book id="1" category="fiction"><title lang="en">XML Guide</title><author>John Doe</author></book></bookstore>)";
    Engine engine(spdlog::level::trace);
    engine.load(directive);
    engine.init();
    auto t = engine.makeTransaction();
    t->processUri(uri_, "POST", version_);
    t->processRequestHeaders(request_header_find_, request_header_traversal_,
                             request_headers_.size(), nullptr);
    t->processRequestBody(xml_body);

    Variable::ArgsPostNames all("", false, false, "");
    result.clear();
    all.evaluate(*t, result);
    EXPECT_EQ(result.size(), 2);

    Variable::ArgsPostNames all_count("", false, true, "");
    result.clear();
    all_count.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 2);

    Variable::ArgsPostNames sub1("title", false, false, "");
    result.clear();
    sub1.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<std::string_view>(result.front().variant_), "title");

    Variable::ArgsPostNames sub2("author", false, false, "");
    result.clear();
    sub2.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<std::string_view>(result.front().variant_), "author");

    Variable::ArgsPostNames sub_count("title", false, true, "");
    result.clear();
    sub_count.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 1);

    {
      Variable::ArgsPostNames sub_regex("/author/", false, false, "");
      result.clear();
      sub_regex.evaluate(*t, result);
      EXPECT_EQ(result.size(), 1);
    }

    {
      Variable::ArgsPostNames sub_regex("/title/", false, false, "");
      result.clear();
      sub_regex.evaluate(*t, result);
      EXPECT_EQ(result.size(), 1);
    }

    {
      Variable::ArgsPostNames sub_regex("/^(t|a)/", false, false, "");
      result.clear();
      sub_regex.evaluate(*t, result);
      EXPECT_EQ(result.size(), 2);
    }

    {
      Variable::ArgsPostNames sub_regex("@test/integration/01_variable_test.data@", false, false,
                                        "");
      result.clear();
      sub_regex.evaluate(*t, result);
      EXPECT_EQ(result.size(), 1);
    }
  }
}

TEST_F(VariableTest, ARGS_POST) {
  Common::EvaluateResults result;
  {
    const std::string directive = R"(
    SecRuleEngine On
    SecAction "id:100,phase:1,ctl:requestBodyProcessor=URLENCODED"
  )";

    std::string urlencoded_body = "p5=v5&p6=v6&p7=v7";
    request_headers_.emplace("content-type", "application/x-www-form-urlencoded");
    Engine engine(spdlog::level::off);
    engine.load(directive);
    engine.init();
    auto t = engine.makeTransaction();
    t->processUri(uri_, "POST", version_);
    t->processRequestHeaders(request_header_find_, request_header_traversal_,
                             request_headers_.size(), nullptr);
    t->processRequestBody(urlencoded_body);

    Variable::ArgsPost all("", false, false, "");
    result.clear();
    all.evaluate(*t, result);
    EXPECT_EQ(result.size(), 3);

    Variable::ArgsPost all_count("", false, true, "");
    result.clear();
    all_count.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 3);

    Variable::ArgsPost sub("p5", false, false, "");
    result.clear();
    sub.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<std::string_view>(result.front().variant_), "v5");

    Variable::ArgsPost sub_count("p5", false, true, "");
    result.clear();
    sub_count.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 1);

    {
      Variable::ArgsPost sub_regex("/^p/", false, false, "");
      result.clear();
      sub_regex.evaluate(*t, result);
      EXPECT_EQ(result.size(), 3);
    }

    {
      Variable::ArgsPost sub_regex("/^p\\d/", false, false, "");
      result.clear();
      sub_regex.evaluate(*t, result);
      EXPECT_EQ(result.size(), 3);
    }

    {
      Variable::ArgsPost sub_regex("/^pa/", false, false, "");
      result.clear();
      sub_regex.evaluate(*t, result);
      EXPECT_EQ(result.size(), 0);
    }

    {
      Variable::ArgsPost sub_regex("@test/integration/01_variable_test.data@", false, false, "");
      result.clear();
      sub_regex.evaluate(*t, result);
      EXPECT_EQ(result.size(), 0);
    }
    request_headers_.erase("content-type");
  }

  {
    const std::string directive = R"(
    SecRuleEngine On
    SecAction "id:100,phase:1,ctl:requestBodyProcessor=MULTIPART"
  )";

    std::string multipart_body = "----helloworld\r\n"
                                 "content-disposition: form-data; name=foo1\r\n"
                                 "header1: value1\r\n"
                                 "\r\n"
                                 "bar1\r\n"
                                 "----helloworld\r\n"
                                 "content-disposition: form-data; name=foo2\r\n"
                                 "header2: value2\r\n"
                                 "\r\n"
                                 "bar2\r\n"
                                 "----helloworld\r\n"
                                 "content-disposition: form-data; name=foo3\r\n"
                                 "header2: value3\r\n"
                                 "\r\n"
                                 "bar3\r\n"
                                 "----helloworld\r\n"
                                 "content-disposition: form-data; name=file1; filename=hello1\r\n"
                                 "\r\n"
                                 "world\r\n"
                                 "----helloworld\r\n"
                                 "content-disposition: form-data; name=file2; filename=hello2\r\n"
                                 "\r\n"
                                 "world\r\n"
                                 "----helloworld\r\n"
                                 "content-disposition: form-data; name=file3; filename=hello3\r\n"
                                 "\r\n"
                                 "world\r\n"
                                 "----helloworld--";

    request_headers_.emplace("content-type", R"(multipart/form-data; boundary=--helloworld)");
    Engine engine(spdlog::level::off);
    engine.load(directive);
    engine.init();
    auto t = engine.makeTransaction();

    t->processUri(uri_, "POST", version_);
    t->processRequestHeaders(request_header_find_, request_header_traversal_,
                             request_headers_.size(), nullptr);
    t->processRequestBody(multipart_body);

    Variable::ArgsPost all("", false, false, "");
    result.clear();
    all.evaluate(*t, result);
    EXPECT_EQ(result.size(), 3);

    Variable::ArgsPost all_count("", false, true, "");
    result.clear();
    all_count.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 3);

    Variable::ArgsPost sub("foo1", false, false, "");
    result.clear();
    sub.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<std::string_view>(result.front().variant_), "bar1\r\n");

    Variable::ArgsPost sub_count("foo1", false, true, "");
    result.clear();
    sub_count.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 1);

    {
      Variable::ArgsPost sub_regex("/^foo/", false, false, "");
      result.clear();
      sub_regex.evaluate(*t, result);
      EXPECT_EQ(result.size(), 3);
    }

    {
      Variable::ArgsPost sub_regex("/^foo\\d/", false, false, "");
      result.clear();
      sub_regex.evaluate(*t, result);
      EXPECT_EQ(result.size(), 3);
    }

    {
      Variable::ArgsPost sub_regex("/^fa/", false, false, "");
      result.clear();
      sub_regex.evaluate(*t, result);
      EXPECT_EQ(result.size(), 0);
    }

    {
      Variable::ArgsPost sub_regex("@test/integration/01_variable_test.data@", false, false, "");
      result.clear();
      sub_regex.evaluate(*t, result);
      EXPECT_EQ(result.size(), 0);
    }

    request_headers_.erase("content-type");
  }

  {
    const std::string directive = R"(
    SecRuleEngine On
    SecAction "id:100,phase:1,ctl:requestBodyProcessor=XML"
    SecParseXmlIntoArgs On
  )";

    std::string_view xml_body =
        R"(<bookstore><book id="1" category="fiction"><title lang="en">XML Guide</title><author>John Doe</author></book></bookstore>)";
    Engine engine(spdlog::level::trace);
    engine.load(directive);
    engine.init();
    auto t = engine.makeTransaction();
    t->processUri(uri_, "POST", version_);
    t->processRequestHeaders(request_header_find_, request_header_traversal_,
                             request_headers_.size(), nullptr);
    t->processRequestBody(xml_body);

    Variable::ArgsPost all("", false, false, "");
    result.clear();
    all.evaluate(*t, result);
    EXPECT_EQ(result.size(), 2);

    Variable::ArgsPost all_count("", false, true, "");
    result.clear();
    all_count.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 2);

    Variable::ArgsPost sub1("title", false, false, "");
    result.clear();
    sub1.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<std::string_view>(result.front().variant_), "XML Guide");

    Variable::ArgsPost sub2("author", false, false, "");
    result.clear();
    sub2.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<std::string_view>(result.front().variant_), "John Doe");

    Variable::ArgsPost sub_count("title", false, true, "");
    result.clear();
    sub_count.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 1);

    {
      Variable::ArgsPost sub_regex("/author/", false, false, "");
      result.clear();
      sub_regex.evaluate(*t, result);
      EXPECT_EQ(result.size(), 1);
    }

    {
      Variable::ArgsPost sub_regex("/title/", false, false, "");
      result.clear();
      sub_regex.evaluate(*t, result);
      EXPECT_EQ(result.size(), 1);
    }

    {
      Variable::ArgsPost sub_regex("/^(t|a)/", false, false, "");
      result.clear();
      sub_regex.evaluate(*t, result);
      EXPECT_EQ(result.size(), 2);
    }

    {
      Variable::ArgsPost sub_regex("@test/integration/01_variable_test.data@", false, false, "");
      result.clear();
      sub_regex.evaluate(*t, result);
      EXPECT_EQ(result.size(), 1);
    }
  }
}

TEST_F(VariableTest, ARGS) {
  Common::EvaluateResults result;
  {
    const std::string directive = R"(
    SecRuleEngine On
    SecAction "id:100,phase:1,ctl:requestBodyProcessor=URLENCODED"
  )";

    std::string urlencoded_body = "p5=v5&p6=v6&p7=v7";
    request_headers_.emplace("content-type", "application/x-www-form-urlencoded");
    Engine engine(spdlog::level::off);
    engine.load(directive);
    engine.init();
    auto t = engine.makeTransaction();
    t->processUri(uri_, "POST", version_);
    t->processRequestHeaders(request_header_find_, request_header_traversal_,
                             request_headers_.size(), nullptr);
    t->processRequestBody(urlencoded_body);

    Variable::Args all("", false, false, "");
    result.clear();
    all.evaluate(*t, result);
    EXPECT_EQ(result.size(), 7);

    Variable::Args all_count("", false, true, "");
    result.clear();
    all_count.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 7);

    Variable::Args sub("p5", false, false, "");
    result.clear();
    sub.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<std::string_view>(result.front().variant_), "v5");

    Variable::Args sub_count("p5", false, true, "");
    result.clear();
    sub_count.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 1);

    {
      Variable::Args sub_regex("/^p/", false, false, "");
      result.clear();
      sub_regex.evaluate(*t, result);
      EXPECT_EQ(result.size(), 7);
    }

    {
      Variable::Args sub_regex("/^p\\d/", false, false, "");
      result.clear();
      sub_regex.evaluate(*t, result);
      EXPECT_EQ(result.size(), 7);
    }

    {
      Variable::Args sub_regex("/^pa/", false, false, "");
      result.clear();
      sub_regex.evaluate(*t, result);
      EXPECT_EQ(result.size(), 0);
    }

    {
      Variable::Args sub_regex("@test/integration/01_variable_test.data@", false, false, "");
      result.clear();
      sub_regex.evaluate(*t, result);
      EXPECT_EQ(result.size(), 2);
    }
    request_headers_.erase("content-type");
  }

  {
    const std::string directive = R"(
    SecRuleEngine On
    SecAction "id:100,phase:1,ctl:requestBodyProcessor=JSON"
  )";

    std::string json_body = R"(
    {
      "p8": "v8",
      "p9": "v9"
    }
    )";
    Engine engine(spdlog::level::off);
    engine.load(directive);
    engine.init();
    auto t = engine.makeTransaction();
    t->processUri(uri_, "POST", version_);
    t->processRequestHeaders(request_header_find_, request_header_traversal_,
                             request_headers_.size(), nullptr);
    t->processRequestBody(json_body);

    Variable::Args all("", false, false, "");
    result.clear();
    all.evaluate(*t, result);
    EXPECT_EQ(result.size(), 6);

    Variable::Args all_count("", false, true, "");
    result.clear();
    all_count.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 6);

    Variable::Args sub("p8", false, false, "");
    result.clear();
    sub.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<std::string_view>(result.front().variant_), "v8");

    Variable::Args sub_count("p8", false, true, "");
    result.clear();
    sub_count.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 1);

    {
      Variable::Args sub_regex("/^p/", false, false, "");
      result.clear();
      sub_regex.evaluate(*t, result);
      EXPECT_EQ(result.size(), 6);
    }

    {
      Variable::Args sub_regex("/^p\\d/", false, false, "");
      result.clear();
      sub_regex.evaluate(*t, result);
      EXPECT_EQ(result.size(), 6);
    }

    {
      Variable::Args sub_regex("/^pa/", false, false, "");
      result.clear();
      sub_regex.evaluate(*t, result);
      EXPECT_EQ(result.size(), 0);
    }

    {
      Variable::Args sub_regex("@test/integration/01_variable_test.data@", false, false, "");
      result.clear();
      sub_regex.evaluate(*t, result);
      EXPECT_EQ(result.size(), 2);
    }
  }

  {
    const std::string directive = R"(
    SecRuleEngine On
    SecAction "id:100,phase:1,ctl:requestBodyProcessor=MULTIPART"
  )";

    std::string multipart_body = "----helloworld\r\n"
                                 "content-disposition: form-data; name=foo1\r\n"
                                 "header1: value1\r\n"
                                 "\r\n"
                                 "bar1\r\n"
                                 "----helloworld\r\n"
                                 "content-disposition: form-data; name=foo2\r\n"
                                 "header2: value2\r\n"
                                 "\r\n"
                                 "bar2\r\n"
                                 "----helloworld\r\n"
                                 "content-disposition: form-data; name=foo3\r\n"
                                 "header2: value3\r\n"
                                 "\r\n"
                                 "bar3\r\n"
                                 "----helloworld\r\n"
                                 "content-disposition: form-data; name=file1; filename=hello1\r\n"
                                 "\r\n"
                                 "world\r\n"
                                 "----helloworld\r\n"
                                 "content-disposition: form-data; name=file2; filename=hello2\r\n"
                                 "\r\n"
                                 "world\r\n"
                                 "----helloworld\r\n"
                                 "content-disposition: form-data; name=file3; filename=hello3\r\n"
                                 "\r\n"
                                 "world\r\n"
                                 "----helloworld--";

    request_headers_.emplace("content-type", R"(multipart/form-data; boundary=--helloworld)");
    Engine engine(spdlog::level::off);
    engine.load(directive);
    engine.init();
    auto t = engine.makeTransaction();

    t->processUri(uri_, "POST", version_);
    t->processRequestHeaders(request_header_find_, request_header_traversal_,
                             request_headers_.size(), nullptr);
    t->processRequestBody(multipart_body);

    Variable::Args all("", false, false, "");
    result.clear();
    all.evaluate(*t, result);
    EXPECT_EQ(result.size(), 7);

    Variable::Args all_count("", false, true, "");
    result.clear();
    all_count.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 7);

    Variable::Args sub("foo1", false, false, "");
    result.clear();
    sub.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<std::string_view>(result.front().variant_), "bar1\r\n");

    Variable::Args sub_count("foo1", false, true, "");
    result.clear();
    sub_count.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 1);

    {
      Variable::Args sub_regex("/^foo/", false, false, "");
      result.clear();
      sub_regex.evaluate(*t, result);
      EXPECT_EQ(result.size(), 3);
    }

    {
      Variable::Args sub_regex("/^foo\\d/", false, false, "");
      result.clear();
      sub_regex.evaluate(*t, result);
      EXPECT_EQ(result.size(), 3);
    }

    {
      Variable::Args sub_regex("/^fa/", false, false, "");
      result.clear();
      sub_regex.evaluate(*t, result);
      EXPECT_EQ(result.size(), 0);
    }

    {
      Variable::Args sub_regex("@test/integration/01_variable_test.data@", false, false, "");
      result.clear();
      sub_regex.evaluate(*t, result);
      EXPECT_EQ(result.size(), 2);
    }

    request_headers_.erase("content-type");
  }

  {
    const std::string directive = R"(
    SecRuleEngine On
    SecAction "id:100,phase:1,ctl:requestBodyProcessor=XML"
    SecParseXmlIntoArgs On
  )";

    std::string_view xml_body =
        R"(<bookstore><book id="1" category="fiction"><title lang="en">XML Guide</title><author>John Doe</author></book></bookstore>)";
    Engine engine(spdlog::level::off);
    engine.load(directive);
    engine.init();
    auto t = engine.makeTransaction();
    t->processUri(uri_, "POST", version_);
    t->processRequestHeaders(request_header_find_, request_header_traversal_,
                             request_headers_.size(), nullptr);
    t->processRequestBody(xml_body);

    Variable::Args all("", false, false, "");
    result.clear();
    all.evaluate(*t, result);
    EXPECT_EQ(result.size(), 6);

    Variable::Args all_count("", false, true, "");
    result.clear();
    all_count.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 6);

    Variable::Args sub1("title", false, false, "");
    result.clear();
    sub1.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<std::string_view>(result.front().variant_), "XML Guide");

    Variable::Args sub2("author", false, false, "");
    result.clear();
    sub2.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<std::string_view>(result.front().variant_), "John Doe");

    Variable::Args sub_count("title", false, true, "");
    result.clear();
    sub_count.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 1);

    {
      Variable::Args sub_regex("/author/", false, false, "");
      result.clear();
      sub_regex.evaluate(*t, result);
      EXPECT_EQ(result.size(), 1);
    }

    {
      Variable::Args sub_regex("/title/", false, false, "");
      result.clear();
      sub_regex.evaluate(*t, result);
      EXPECT_EQ(result.size(), 1);
    }

    {
      Variable::Args sub_regex("/^(t|a)/", false, false, "");
      result.clear();
      sub_regex.evaluate(*t, result);
      EXPECT_EQ(result.size(), 2);
    }

    {
      Variable::Args sub_regex("@test/integration/01_variable_test.data@", false, false, "");
      result.clear();
      sub_regex.evaluate(*t, result);
      EXPECT_EQ(result.size(), 3);
    }
  }
}

TEST_F(VariableTest, AUTH_TYPE) {
  Common::EvaluateResults result;

  // Test with empty Authorization headers (no authentication)
  {
    Variable::AuthType auth_type_no_auth("", false, false, "");
    result.clear();
    auth_type_no_auth.evaluate(*t_, result);
    EXPECT_TRUE(result.empty());
  }

  // Test counter mode with empty Authorization headers
  {
    Variable::AuthType auth_type_count("", false, true, "");
    result.clear();
    auth_type_count.evaluate(*t_, result);
    EXPECT_FALSE(result.empty());
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 0);
  }

  // Test with Basic authentication
  {
    request_headers_.emplace("authorization", "Basic dXNlcjpwYXNzd29yZA==");
    Variable::AuthType auth_type_basic("", false, false, "");
    result.clear();
    auth_type_basic.evaluate(*t_, result);
    EXPECT_FALSE(result.empty());
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<std::string_view>(result.front().variant_), "Basic");
    request_headers_.erase("authorization");
  }

  // Test counter mode with Basic authentication
  {
    request_headers_.emplace("authorization", "Basic dXNlcjpwYXNzd29yZA==");
    Variable::AuthType auth_type_count("", false, true, "");
    result.clear();
    auth_type_count.evaluate(*t_, result);
    EXPECT_FALSE(result.empty());
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 1);
    request_headers_.erase("authorization");
  }

  // Test with multiple Authorization headers (Bearer and Digest)
  {
    request_headers_.emplace("authorization", "Bearer eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9");
    request_headers_.emplace("authorization", "Digest username=\"user\", realm=\"test\"");
    Variable::AuthType auth_type_multi("", false, false, "");
    result.clear();
    auth_type_multi.evaluate(*t_, result);
    EXPECT_FALSE(result.empty());
    EXPECT_EQ(result.size(), 2);

    // Check both auth types are extracted correctly
    std::vector<std::string_view> auth_types;
    for (const auto& res : result) {
      auth_types.push_back(std::get<std::string_view>(res.variant_));
    }

    EXPECT_TRUE(std::find(auth_types.begin(), auth_types.end(), "Bearer") != auth_types.end());
    EXPECT_TRUE(std::find(auth_types.begin(), auth_types.end(), "Digest") != auth_types.end());
    request_headers_.erase("authorization");
  }

  // Test counter mode with multiple Authorization headers
  {
    request_headers_.emplace("authorization", "Bearer eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9");
    request_headers_.emplace("authorization", "Digest username=\"user\", realm=\"test\"");
    Variable::AuthType auth_type_count("", false, true, "");
    result.clear();
    auth_type_count.evaluate(*t_, result);
    EXPECT_FALSE(result.empty());
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 2);
    request_headers_.erase("authorization");
  }
}

TEST_F(VariableTest, DURATION) {
  Common::EvaluateResults result;

  // Test counter mode - should return 1 (transaction has creation timestamp)
  {
    Variable::Duration duration_count("", false, true, "");
    result.clear();
    duration_count.evaluate(*t_, result);
    EXPECT_FALSE(result.empty());
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 1);
  }

  // Test collection mode - should return actual duration in milliseconds
  {
    Variable::Duration duration("", false, false, "");
    // Small sleep to ensure duration increases
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    result.clear();
    duration.evaluate(*t_, result);
    EXPECT_FALSE(result.empty());
    EXPECT_EQ(result.size(), 1);
    // Duration should be a positive value (current time minus creation time)
    auto duration_value = std::get<int64_t>(result.front().variant_);
    EXPECT_GT(duration_value, 0);
  }

  // Test with a fresh transaction to verify duration increases over time
  {
    Engine new_engine(spdlog::level::off);
    new_engine.init();
    auto fresh_t = new_engine.makeTransaction();
    fresh_t->processConnection(downstream_ip_, downstream_port_, upstream_ip_, upstream_port_);
    fresh_t->processUri(uri_, method_, version_);
    fresh_t->processRequestHeaders(request_header_find_, request_header_traversal_,
                                   request_headers_.size(), nullptr);

    // Get initial duration
    Variable::Duration initial_duration("", false, false, "");
    // Small sleep to ensure duration increases
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    result.clear();
    initial_duration.evaluate(*fresh_t, result);
    EXPECT_FALSE(result.empty());
    auto initial_duration_value = std::get<int64_t>(result.front().variant_);
    EXPECT_GT(initial_duration_value, 0);

    // Small sleep to ensure duration increases
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    // Get duration after sleep
    result.clear();
    initial_duration.evaluate(*fresh_t, result);
    EXPECT_FALSE(result.empty());
    auto later_duration_value = std::get<int64_t>(result.front().variant_);

    // Later duration should be greater than initial duration
    EXPECT_GT(later_duration_value, initial_duration_value);
  }
}

TEST_F(VariableTest, ENV) {
  // Store original environment variables for cleanup
  const char* original_test_var1 = std::getenv("WGE_TEST_VAR1");
  const char* original_test_var2 = std::getenv("WGE_TEST_VAR2");
  const char* original_path_like_var = std::getenv("WGE_PATH_VAR");

  // Set up custom environment variables for testing
  setenv("WGE_TEST_VAR1", "test_value_1", 1);
  setenv("WGE_TEST_VAR2", "test_value_2", 1);
  setenv("WGE_PATH_VAR", "/usr/local/bin:/usr/bin", 1);

  // Create a fresh engine and transaction to pick up the new environment variables
  Engine fresh_engine(spdlog::level::off);
  fresh_engine.init();
  fresh_engine.updatePropertyStore(ptree_json_);
  auto fresh_t = fresh_engine.makeTransaction();
  fresh_t->processConnection(downstream_ip_, downstream_port_, upstream_ip_, upstream_port_);
  fresh_t->processUri(uri_, method_, version_);
  fresh_t->processRequestHeaders(request_header_find_, request_header_traversal_,
                                 request_headers_.size(), nullptr);

  Common::EvaluateResults result;

  // Test counter mode - count all environment variables
  {
    Variable::Env env_count("", false, true, "");
    result.clear();
    env_count.evaluate(*fresh_t, result);
    EXPECT_FALSE(result.empty());
    EXPECT_EQ(result.size(), 1);

    // Environment variable count should be positive since we added test vars
    auto count = std::get<int64_t>(result.front().variant_);
    EXPECT_GT(count, 0);
  }

  // Test collection mode - get all environment variables
  {
    Variable::Env env_all("", false, false, "");
    result.clear();
    env_all.evaluate(*fresh_t, result);

    // Should have at least the test environment variables
    EXPECT_FALSE(result.empty());

    // Each result should have both key and value
    for (const auto& res : result) {
      EXPECT_TRUE(IS_STRING_VIEW_VARIANT(res.variant_));
    }
  }

  // Test with our specific test environment variable lookup
  {
    Variable::Env env_var1("WGE_TEST_VAR1", false, false, "");
    result.clear();
    env_var1.evaluate(*fresh_t, result);

    // Our test variable should exist
    EXPECT_FALSE(result.empty());
    EXPECT_EQ(result.size(), 1);
    EXPECT_TRUE(IS_STRING_VIEW_VARIANT(result.front().variant_));
    EXPECT_EQ(std::get<std::string_view>(result.front().variant_), "test_value_1");
  }

  // Test counter mode with specific environment variable
  {
    Variable::Env env_var1_count("WGE_TEST_VAR1", false, true, "");
    result.clear();
    env_var1_count.evaluate(*fresh_t, result);
    EXPECT_FALSE(result.empty());
    EXPECT_EQ(result.size(), 1);

    // Should count 1 since our test variable exists
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 1);
  }

  // Test with another specific test variable
  {
    Variable::Env env_var2("WGE_TEST_VAR2", false, false, "");
    result.clear();
    env_var2.evaluate(*fresh_t, result);

    // Our test variable should exist
    EXPECT_FALSE(result.empty());
    EXPECT_EQ(result.size(), 1);
    EXPECT_TRUE(IS_STRING_VIEW_VARIANT(result.front().variant_));
    EXPECT_EQ(std::get<std::string_view>(result.front().variant_), "test_value_2");
  }

  // Test with non-existent environment variable
  {
    Variable::Env env_nonexistent("WGE_NONEXISTENT_VAR_12345", false, false, "");
    result.clear();
    env_nonexistent.evaluate(*fresh_t, result);

    // Should be empty since the variable doesn't exist
    EXPECT_TRUE(result.empty());
  }

  // Test counter mode with non-existent environment variable
  {
    Variable::Env env_nonexistent_count("WGE_NONEXISTENT_VAR_12345", false, true, "");
    result.clear();
    env_nonexistent_count.evaluate(*fresh_t, result);
    EXPECT_FALSE(result.empty());
    EXPECT_EQ(result.size(), 1);

    // Should count 0 for non-existent variable
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 0);
  }

  // Test regex pattern matching with our custom environment variables
  {
    Variable::Env env_pattern("/^WGE_/", false, false, "");
    result.clear();
    env_pattern.evaluate(*fresh_t, result);

    // Should match our 3 test variables
    EXPECT_GE(result.size(), 3);

    // Verify our test variables are found
    size_t found_test_vars = 0;
    for (const auto& res : result) {
      std::string_view value = std::get<std::string_view>(res.variant_);
      if (value == "test_value_1" || value == "test_value_2" ||
          value == "/usr/local/bin:/usr/bin") {
        found_test_vars++;
      }
    }
    EXPECT_GE(found_test_vars, 3); // At least 2 of our 3 should be found
  }

  // Test counter mode with regex pattern
  {
    Variable::Env env_pattern_count("/^WGE_/", false, true, "");
    result.clear();
    env_pattern_count.evaluate(*fresh_t, result);
    EXPECT_FALSE(result.empty());
    EXPECT_EQ(result.size(), 1);

    // Should match our 3 test variables
    auto count = std::get<int64_t>(result.front().variant_);
    EXPECT_GE(count, 3);
  }

  // Test exact value retrieval with different patterns
  {
    Variable::Env env_path_like("WGE_PATH_VAR", false, false, "");
    result.clear();
    env_path_like.evaluate(*fresh_t, result);

    EXPECT_FALSE(result.empty());
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<std::string_view>(result.front().variant_), "/usr/local/bin:/usr/bin");
  }

  // Clean up - restore original environment variables
  if (original_test_var1) {
    setenv("WGE_TEST_VAR1", original_test_var1, 1);
  } else {
    unsetenv("WGE_TEST_VAR1");
  }

  if (original_test_var2) {
    setenv("WGE_TEST_VAR2", original_test_var2, 1);
  } else {
    unsetenv("WGE_TEST_VAR2");
  }

  if (original_path_like_var) {
    setenv("WGE_PATH_VAR", original_path_like_var, 1);
  } else {
    unsetenv("WGE_PATH_VAR");
  }
}

TEST_F(VariableTest, FILES_COMBINED_SIZE) {
  Common::EvaluateResults result;
  {
    const std::string directive = R"(
    SecRuleEngine On
    SecAction "id:100,phase:1,ctl:requestBodyProcessor=MULTIPART"
  )";

    std::string file_content = "Hello world";

    std::string multipart_body =
        std::format("----helloworld\r\n"
                    "content-disposition: form-data; name=foo1\r\n"
                    "header1: value1\r\n"
                    "\r\n"
                    "bar1\r\n"
                    "----helloworld\r\n"
                    "content-disposition: form-data; name=foo2\r\n"
                    "header2: value2\r\n"
                    "\r\n"
                    "bar2\r\n"
                    "----helloworld\r\n"
                    "content-disposition: form-data; name=foo3\r\n"
                    "header2: value3\r\n"
                    "\r\n"
                    "bar3\r\n"
                    "----helloworld\r\n"
                    "content-disposition: form-data; name=file1; filename=hello1\r\n"
                    "\r\n"
                    "{}\r\n"
                    "----helloworld\r\n"
                    "content-disposition: form-data; name=file2; filename=hello2\r\n"
                    "\r\n"
                    "{}\r\n"
                    "----helloworld\r\n"
                    "content-disposition: form-data; name=file3; filename=hello3\r\n"
                    "\r\n"
                    "{}\r\n"
                    "----helloworld--",
                    file_content, file_content, file_content);

    request_headers_.emplace("content-type", R"(multipart/form-data; boundary=--helloworld)");
    Engine engine(spdlog::level::trace);
    engine.load(directive);
    engine.init();
    auto t = engine.makeTransaction();

    t->processUri(uri_, "POST", version_);
    t->processRequestHeaders(request_header_find_, request_header_traversal_,
                             request_headers_.size(), nullptr);
    t->processRequestBody(multipart_body);

    Variable::FilesCombinedSize files_combineds_size("", false, false, "");
    result.clear();

    files_combineds_size.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_),
              3 * file_content.size() + 3 * strlen("\r\n"));

    request_headers_.erase("content-type");
  }
}

TEST_F(VariableTest, FILES_NAMES) {
  Common::EvaluateResults result;
  {
    const std::string directive = R"(
    SecRuleEngine On
    SecAction "id:100,phase:1,ctl:requestBodyProcessor=MULTIPART"
  )";
    std::vector<std::string> files_names_vec = {"file1", "file2", "file3"};
    std::string multipart_body =
        std::format("----helloworld\r\n"
                    "content-disposition: form-data; name=foo1\r\n"
                    "header1: value1\r\n"
                    "\r\n"
                    "bar1\r\n"
                    "----helloworld\r\n"
                    "content-disposition: form-data; name=foo2\r\n"
                    "header2: value2\r\n"
                    "\r\n"
                    "bar2\r\n"
                    "----helloworld\r\n"
                    "content-disposition: form-data; name=foo3\r\n"
                    "header2: value3\r\n"
                    "\r\n"
                    "bar3\r\n"
                    "----helloworld\r\n"
                    "content-disposition: form-data; name={}; filename=Hello1\r\n"
                    "\r\n"
                    "world\r\n"
                    "----helloworld\r\n"
                    "content-disposition: form-data; name={}; filename=Hello2\r\n"
                    "\r\n"
                    "world\r\n"
                    "----helloworld\r\n"
                    "content-disposition: form-data; name={}; filename=Hello3\r\n"
                    "\r\n"
                    "world\r\n"
                    "----helloworld--",
                    files_names_vec[0], files_names_vec[1], files_names_vec[2]);

    request_headers_.emplace("content-type", R"(multipart/form-data; boundary=--helloworld)");
    Engine engine(spdlog::level::off);
    engine.load(directive);
    engine.init();
    auto t = engine.makeTransaction();

    t->processUri(uri_, "POST", version_);
    t->processRequestHeaders(request_header_find_, request_header_traversal_,
                             request_headers_.size(), nullptr);
    t->processRequestBody(multipart_body);

    {
      Variable::FilesNames all("", false, false, "");
      result.clear();

      all.evaluate(*t, result);
      EXPECT_EQ(result.size(), files_names_vec.size());

      for (size_t i = 0; i < result.size(); ++i) {
        EXPECT_EQ(std::get<std::string_view>(result[i].variant_), files_names_vec[i]);
      }
    }

    {
      Variable::FilesNames all_count("", false, true, "");
      result.clear();

      all_count.evaluate(*t, result);
      EXPECT_EQ(result.size(), 1);
      EXPECT_EQ(std::get<int64_t>(result.front().variant_), files_names_vec.size());
    }

    {
      Variable::FilesNames sub_regex("/^file/", false, false, "");
      result.clear();

      sub_regex.evaluate(*t, result);
      EXPECT_EQ(result.size(), files_names_vec.size());
      for (size_t i = 0; i < result.size(); ++i) {
        EXPECT_EQ(std::get<std::string_view>(result[i].variant_), files_names_vec[i]);
      }
    }
    request_headers_.erase("content-type");
  }
}

TEST_F(VariableTest, FILES_SIZES) {
  Common::EvaluateResults result;
  {
    const std::string directive = R"(
    SecRuleEngine On
    SecAction "id:100,phase:1,ctl:requestBodyProcessor=MULTIPART"
  )";

    std::vector<std::string> file_content = {"Hello", "World", "!"};

    std::string multipart_body =
        std::format("----helloworld\r\n"
                    "content-disposition: form-data; name=foo1\r\n"
                    "header1: value1\r\n"
                    "\r\n"
                    "bar1\r\n"
                    "----helloworld\r\n"
                    "content-disposition: form-data; name=foo2\r\n"
                    "header2: value2\r\n"
                    "\r\n"
                    "bar2\r\n"
                    "----helloworld\r\n"
                    "content-disposition: form-data; name=foo3\r\n"
                    "header2: value3\r\n"
                    "\r\n"
                    "bar3\r\n"
                    "----helloworld\r\n"
                    "content-disposition: form-data; name=file1; filename=hello1\r\n"
                    "\r\n"
                    "{}\r\n"
                    "----helloworld\r\n"
                    "content-disposition: form-data; name=file2; filename=hello2\r\n"
                    "\r\n"
                    "{}\r\n"
                    "----helloworld\r\n"
                    "content-disposition: form-data; name=file3; filename=hello3\r\n"
                    "\r\n"
                    "{}\r\n"
                    "----helloworld--",
                    file_content[0], file_content[1], file_content[2]);

    request_headers_.emplace("content-type", R"(multipart/form-data; boundary=--helloworld)");
    Engine engine(spdlog::level::trace);
    engine.load(directive);
    engine.init();
    auto t = engine.makeTransaction();

    t->processUri(uri_, "POST", version_);
    t->processRequestHeaders(request_header_find_, request_header_traversal_,
                             request_headers_.size(), nullptr);
    t->processRequestBody(multipart_body);

    {
      Variable::FilesSizes all("", false, false, "");
      result.clear();

      all.evaluate(*t, result);
      EXPECT_EQ(result.size(), file_content.size());
      for (size_t i = 0; i < result.size(); ++i) {
        EXPECT_EQ(std::get<int64_t>(result[i].variant_), file_content[i].size() + strlen("\r\n"));
      }
    }

    {
      Variable::FilesSizes all_count("", false, true, "");
      result.clear();

      all_count.evaluate(*t, result);
      EXPECT_EQ(result.size(), 1);
      EXPECT_EQ(std::get<int64_t>(result.front().variant_), file_content.size());
    }

    request_headers_.erase("content-type");
  }
}

TEST_F(VariableTest, FILES_TMPNAMES) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, FILES_TMP_CONTENT) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, FILES) {
  Common::EvaluateResults result;
  {
    const std::string directive = R"(
    SecRuleEngine On
    SecAction "id:100,phase:1,ctl:requestBodyProcessor=MULTIPART"
  )";
    std::vector<std::string> files_vec = {"Hello1", "Hello2", "Hello3"};
    std::string multipart_body =
        std::format("----helloworld\r\n"
                    "content-disposition: form-data; name=foo1\r\n"
                    "header1: value1\r\n"
                    "\r\n"
                    "bar1\r\n"
                    "----helloworld\r\n"
                    "content-disposition: form-data; name=foo2\r\n"
                    "header2: value2\r\n"
                    "\r\n"
                    "bar2\r\n"
                    "----helloworld\r\n"
                    "content-disposition: form-data; name=foo3\r\n"
                    "header2: value3\r\n"
                    "\r\n"
                    "bar3\r\n"
                    "----helloworld\r\n"
                    "content-disposition: form-data; name=file1; filename={}\r\n"
                    "\r\n"
                    "world\r\n"
                    "----helloworld\r\n"
                    "content-disposition: form-data; name=file2; filename={}\r\n"
                    "\r\n"
                    "world\r\n"
                    "----helloworld\r\n"
                    "content-disposition: form-data; name=file3; filename={}\r\n"
                    "\r\n"
                    "world\r\n"
                    "----helloworld--",
                    files_vec[0], files_vec[1], files_vec[2]);

    request_headers_.emplace("content-type", R"(multipart/form-data; boundary=--helloworld)");
    Engine engine(spdlog::level::off);
    engine.load(directive);
    engine.init();
    auto t = engine.makeTransaction();

    t->processUri(uri_, "POST", version_);
    t->processRequestHeaders(request_header_find_, request_header_traversal_,
                             request_headers_.size(), nullptr);
    t->processRequestBody(multipart_body);

    {
      Variable::Files all("", false, false, "");
      result.clear();

      all.evaluate(*t, result);
      EXPECT_EQ(result.size(), files_vec.size());

      for (size_t i = 0; i < result.size(); ++i) {
        EXPECT_EQ(std::get<std::string_view>(result[i].variant_), files_vec[i]);
      }
    }

    {
      Variable::Files all_count("", false, true, "");
      result.clear();

      all_count.evaluate(*t, result);
      EXPECT_EQ(result.size(), 1);
      EXPECT_EQ(std::get<int64_t>(result.front().variant_), files_vec.size());
    }

    {
      Variable::Files sub_regex("/^file/", false, false, "");
      result.clear();

      sub_regex.evaluate(*t, result);
      EXPECT_EQ(result.size(), files_vec.size());
      for (size_t i = 0; i < result.size(); ++i) {
        EXPECT_EQ(std::get<std::string_view>(result[i].variant_), files_vec[i]);
      }
    }
    request_headers_.erase("content-type");
  }
}

TEST_F(VariableTest, FULL_REQUEST_LENGTH) {
  Common::EvaluateResults result;
  {
    const std::string directive = R"(
    SecRuleEngine On
    SecAction "id:100,phase:1,ctl:requestBodyProcessor=MULTIPART"
  )";

    std::string multipart_body = "----helloworld\r\n"
                                 "content-disposition: form-data; name=foo1\r\n"
                                 "header1: value1\r\n"
                                 "\r\n"
                                 "bar1\r\n"
                                 "----helloworld\r\n"
                                 "content-disposition: form-data; name=foo2\r\n"
                                 "header2: value2\r\n"
                                 "\r\n"
                                 "bar2\r\n"
                                 "----helloworld\r\n"
                                 "content-disposition: form-data; name=foo3\r\n"
                                 "header2: value3\r\n"
                                 "\r\n"
                                 "bar3\r\n"
                                 "----helloworld\r\n"
                                 "content-disposition: form-data; name=file1; filename=hello1\r\n"
                                 "\r\n"
                                 "world\r\n"
                                 "----helloworld\r\n"
                                 "content-disposition: form-data; name=file2; filename=hello2\r\n"
                                 "\r\n"
                                 "world\r\n"
                                 "----helloworld\r\n"
                                 "content-disposition: form-data; name=file3; filename=hello3\r\n"
                                 "\r\n"
                                 "world\r\n"
                                 "----helloworld--";

    request_headers_.emplace("content-type", R"(multipart/form-data; boundary=--helloworld)");
    Engine engine(spdlog::level::off);
    engine.load(directive);
    engine.init();
    auto t = engine.makeTransaction();

    t->processUri(uri_, "POST", version_);
    t->processRequestHeaders(request_header_find_, request_header_traversal_,
                             request_headers_.size(), nullptr);
    t->processRequestBody(multipart_body);

    Variable::FullRequestLength request_length("", false, false, "");
    result.clear();

    request_length.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);

    size_t length = 0;
    for (auto& [key, value] : request_headers_) {
      length += key.size() + strlen(": ") + value.size() + strlen("\n");
    }
    length += strlen("\n\n");
    length += multipart_body.size();

    EXPECT_EQ(std::get<int64_t>(result.front().variant_), length);

    request_headers_.erase("content-type");
  }
}

TEST_F(VariableTest, FULL_REQUEST) {
  Common::EvaluateResults result;
  {
    const std::string directive = R"(
    SecRuleEngine On
    SecAction "id:100,phase:1,ctl:requestBodyProcessor=MULTIPART"
  )";

    std::string multipart_body = "----helloworld\r\n"
                                 "content-disposition: form-data; name=foo1\r\n"
                                 "header1: value1\r\n"
                                 "\r\n"
                                 "bar1\r\n"
                                 "----helloworld\r\n"
                                 "content-disposition: form-data; name=foo2\r\n"
                                 "header2: value2\r\n"
                                 "\r\n"
                                 "bar2\r\n"
                                 "----helloworld\r\n"
                                 "content-disposition: form-data; name=foo3\r\n"
                                 "header2: value3\r\n"
                                 "\r\n"
                                 "bar3\r\n"
                                 "----helloworld\r\n"
                                 "content-disposition: form-data; name=file1; filename=hello1\r\n"
                                 "\r\n"
                                 "world\r\n"
                                 "----helloworld\r\n"
                                 "content-disposition: form-data; name=file2; filename=hello2\r\n"
                                 "\r\n"
                                 "world\r\n"
                                 "----helloworld\r\n"
                                 "content-disposition: form-data; name=file3; filename=hello3\r\n"
                                 "\r\n"
                                 "world\r\n"
                                 "----helloworld--";

    request_headers_.emplace("content-type", R"(multipart/form-data; boundary=--helloworld)");
    Engine engine(spdlog::level::off);
    engine.load(directive);
    engine.init();
    auto t = engine.makeTransaction();

    t->processUri(uri_, "POST", version_);
    t->processRequestHeaders(request_header_find_, request_header_traversal_,
                             request_headers_.size(), nullptr);
    t->processRequestBody(multipart_body);

    Variable::FullRequest request("", false, false, "");
    result.clear();

    request.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);

    for (auto& [key, value] : request_headers_) {
      EXPECT_TRUE(std::get<std::string_view>(result.front().variant_).contains(key));
      EXPECT_TRUE(std::get<std::string_view>(result.front().variant_).contains(value));
    }
    EXPECT_TRUE(std::get<std::string_view>(result.front().variant_).contains(multipart_body));
    request_headers_.erase("content-type");
  }
}

TEST_F(VariableTest, GEO) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, HIGHEST_SEVERITY) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, INBOUND_DATA_ERROR) {
  Common::EvaluateResults result;

  // Test URL-Encoded body processor with body size below limit
  {
    const std::string directive = R"(
    SecRuleEngine On
    SecRequestBodyLimit 1024
    SecAction "id:100,phase:1,ctl:requestBodyProcessor=URLENCODED"
  )";

    std::string urlencoded_body_below = "p1=v1&p2=v2&p3=v3";
    Engine engine(spdlog::level::off);
    engine.load(directive);
    engine.init();
    auto t = engine.makeTransaction();
    t->processUri(uri_, "POST", version_);
    t->processRequestHeaders(request_header_find_, request_header_traversal_,
                             request_headers_.size(), nullptr);
    t->processRequestBody(urlencoded_body_below);

    Variable::InboundDataError inbound_data_error("", false, false, "");
    result.clear();
    inbound_data_error.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 0);
  }

  // Test URL-Encoded body processor with body size above limit
  {
    const std::string directive = R"(
    SecRuleEngine On
    SecRequestBodyLimit 10
    SecAction "id:100,phase:1,ctl:requestBodyProcessor=URLENCODED"
  )";

    std::string urlencoded_body_above =
        "p1=very_long_value1&p2=very_long_value2&p3=very_long_value3";
    Engine engine(spdlog::level::off);
    engine.load(directive);
    engine.init();
    auto t = engine.makeTransaction();
    t->processUri(uri_, "POST", version_);
    t->processRequestHeaders(request_header_find_, request_header_traversal_,
                             request_headers_.size(), nullptr);
    t->processRequestBody(urlencoded_body_above);

    Variable::InboundDataError inbound_data_error("", false, false, "");
    result.clear();
    inbound_data_error.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 1);
  }

  // Test JSON body processor with body size below limit
  {
    const std::string directive = R"(
    SecRuleEngine On
    SecRequestBodyLimit 1024
    SecAction "id:100,phase:1,ctl:requestBodyProcessor=JSON"
  )";

    std::string json_body_below = R"({
      "field1": "value1",
      "field2": "value2"
    })";
    Engine engine(spdlog::level::off);
    engine.load(directive);
    engine.init();
    auto t = engine.makeTransaction();
    t->processUri(uri_, "POST", version_);
    t->processRequestHeaders(request_header_find_, request_header_traversal_,
                             request_headers_.size(), nullptr);
    t->processRequestBody(json_body_below);

    Variable::InboundDataError inbound_data_error("", false, false, "");
    result.clear();
    inbound_data_error.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 0);
  }

  // Test JSON body processor with body size above limit
  {
    const std::string directive = R"(
    SecRuleEngine On
    SecRequestBodyLimit 50
    SecAction "id:100,phase:1,ctl:requestBodyProcessor=JSON"
  )";

    std::string json_body_above = R"({
      "field1": "very_long_value_that_exceeds_the_limit_1234567890",
      "field2": "another_very_long_value_that_also_exceeds_1234567890"
    })";
    Engine engine(spdlog::level::off);
    engine.load(directive);
    engine.init();
    auto t = engine.makeTransaction();
    t->processUri(uri_, "POST", version_);
    t->processRequestHeaders(request_header_find_, request_header_traversal_,
                             request_headers_.size(), nullptr);
    t->processRequestBody(json_body_above);

    Variable::InboundDataError inbound_data_error("", false, false, "");
    result.clear();
    inbound_data_error.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 1);
  }

  // Test Multipart body processor with body size below no-files limit
  {
    const std::string directive = R"(
    SecRuleEngine On
    SecRequestBodyNoFilesLimit 1024
    SecAction "id:100,phase:1,ctl:requestBodyProcessor=MULTIPART"
  )";

    std::string multipart_body_below = "----helloworld\r\n"
                                       "content-disposition: form-data; name=field1\r\n"
                                       "header1: value1\r\n"
                                       "\r\n"
                                       "value1\r\n"
                                       "----helloworld--\r\n";
    request_headers_.emplace("content-type", "multipart/form-data; boundary=--helloworld");
    Engine engine(spdlog::level::off);
    engine.load(directive);
    engine.init();
    auto t = engine.makeTransaction();
    t->processUri(uri_, "POST", version_);
    t->processRequestHeaders(request_header_find_, request_header_traversal_,
                             request_headers_.size(), nullptr);
    t->processRequestBody(multipart_body_below);

    Variable::RequestBody req_body("", false, false, "");
    Common::EvaluateResults req_body_result;
    req_body.evaluate(*t, req_body_result);
    EXPECT_EQ(req_body_result.size(), 1);
    EXPECT_EQ(std::get<std::string_view>(req_body_result.front().variant_), multipart_body_below);

    Variable::InboundDataError inbound_data_error("", false, false, "");
    result.clear();
    inbound_data_error.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 0);
    request_headers_.erase("content-type");
  }

  // Test Multipart body processor with body size above no-files limit
  {
    const std::string directive = R"(
    SecRuleEngine On
    SecRequestBodyNoFilesLimit 30
    SecAction "id:100,phase:1,ctl:requestBodyProcessor=MULTIPART"
  )";

    std::string multipart_body_above = "----helloworld\r\n"
                                       "content-disposition: form-data; name=field1\r\n"
                                       "header1: value1\r\n"
                                       "\r\n"
                                       "this_value_is_long_enough_to_exceed_the_limit_123\r\n"
                                       "----helloworld--\r\n";
    request_headers_.emplace("content-type", "multipart/form-data; boundary=--helloworld");
    Engine engine(spdlog::level::off);
    engine.load(directive);
    engine.init();
    auto t = engine.makeTransaction();
    t->processUri(uri_, "POST", version_);
    t->processRequestHeaders(request_header_find_, request_header_traversal_,
                             request_headers_.size(), nullptr);
    t->processRequestBody(multipart_body_above);

    Variable::InboundDataError inbound_data_error("", false, false, "");
    result.clear();
    inbound_data_error.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 1);
    request_headers_.erase("content-type");
  }

  // Test counter mode with no error
  {
    const std::string directive = R"(
    SecRuleEngine On
    SecRequestBodyLimit 1024
    SecAction "id:100,phase:1,ctl:requestBodyProcessor=URLENCODED"
  )";

    std::string urlencoded_body = "p1=v1";
    Engine engine(spdlog::level::off);
    engine.load(directive);
    engine.init();
    auto t = engine.makeTransaction();
    t->processUri(uri_, "POST", version_);
    t->processRequestHeaders(request_header_find_, request_header_traversal_,
                             request_headers_.size(), nullptr);
    t->processRequestBody(urlencoded_body);

    Variable::InboundDataError inbound_data_error_count("", false, true, "");
    result.clear();
    inbound_data_error_count.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 0);
  }

  // Test counter mode with error
  {
    const std::string directive = R"(
    SecRuleEngine On
    SecRequestBodyLimit 5
    SecAction "id:100,phase:1,ctl:requestBodyProcessor=URLENCODED"
  )";

    std::string urlencoded_body = "p1=very_long_value";
    Engine engine(spdlog::level::off);
    engine.load(directive);
    engine.init();
    auto t = engine.makeTransaction();
    t->processUri(uri_, "POST", version_);
    t->processRequestHeaders(request_header_find_, request_header_traversal_,
                             request_headers_.size(), nullptr);
    t->processRequestBody(urlencoded_body);

    Variable::InboundDataError inbound_data_error_count("", false, true, "");
    result.clear();
    inbound_data_error_count.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 1);
  }

  // Test XML body processor with body size below limit
  {
    const std::string directive = R"(
    SecRuleEngine On
    SecRequestBodyLimit 1024
    SecAction "id:100,phase:1,ctl:requestBodyProcessor=XML"
  )";

    std::string_view xml_body_below = "<root><element>value</element></root>";
    Engine engine(spdlog::level::off);
    engine.load(directive);
    engine.init();
    auto t = engine.makeTransaction();
    t->processUri(uri_, "POST", version_);
    t->processRequestHeaders(request_header_find_, request_header_traversal_,
                             request_headers_.size(), nullptr);
    t->processRequestBody(xml_body_below);

    Variable::InboundDataError inbound_data_error("", false, false, "");
    result.clear();
    inbound_data_error.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 0);
  }

  // Test XML body processor with body size above limit
  {
    const std::string directive = R"(
    SecRuleEngine On
    SecRequestBodyLimit 20
    SecAction "id:100,phase:1,ctl:requestBodyProcessor=XML"
  )";

    std::string_view xml_body_above =
        "<root><very_long_element_name>content</very_long_element_name></root>";
    Engine engine(spdlog::level::off);
    engine.load(directive);
    engine.init();
    auto t = engine.makeTransaction();
    t->processUri(uri_, "POST", version_);
    t->processRequestHeaders(request_header_find_, request_header_traversal_,
                             request_headers_.size(), nullptr);
    t->processRequestBody(xml_body_above);

    Variable::InboundDataError inbound_data_error("", false, false, "");
    result.clear();
    inbound_data_error.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 1);
  }
}

TEST_F(VariableTest, MATCHED_VAR_NAME) {
  Common::EvaluateResults result;
  {
    const std::string directive = R"(
    SecRuleEngine On
    SecRule ARGS "@unconditionalMatch" "id:200,phase:1,chain"
      SecRule MATCHED_VAR "@unconditionalMatch" "t:none"
  )";

    Engine engine(spdlog::level::off);
    engine.load(directive);
    engine.init();
    auto t = engine.makeTransaction();
    t->processUri(uri_, method_, version_);
    t->processRequestHeaders(request_header_find_, request_header_traversal_,
                             request_headers_.size(), nullptr);

    Variable::MatchedVarName matched_var_name("", false, false, "");
    result.clear();
    matched_var_name.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);

    EXPECT_EQ(std::get<std::string_view>(result.front().variant_), "ARGS:p4");
  }
}

TEST_F(VariableTest, MATCHED_VAR) {
  Common::EvaluateResults result;
  {
    const std::string directive = R"(
    SecRuleEngine On
    SecRule ARGS "@unconditionalMatch" "id:200,phase:1,chain"
      SecRule MATCHED_VAR "@unconditionalMatch" "t:none"
  )";

    Engine engine(spdlog::level::off);
    engine.load(directive);
    engine.init();
    auto t = engine.makeTransaction();
    t->processUri(uri_, method_, version_);
    t->processRequestHeaders(request_header_find_, request_header_traversal_,
                             request_headers_.size(), nullptr);

    Variable::MatchedVar matched_var("", false, false, "");
    result.clear();
    matched_var.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);

    EXPECT_EQ(std::get<std::string_view>(result.front().variant_), "v4");
  }
}

TEST_F(VariableTest, MATCHED_VARS_NAMES) {
  Common::EvaluateResults result;
  {
    const std::string directive = R"(
    SecRuleEngine On
    SecRule ARGS "@unconditionalMatch" "id:200,phase:1,chain"
      SecRule MATCHED_VARS "@unconditionalMatch" "t:none"
  )";

    Engine engine(spdlog::level::off);
    engine.load(directive);
    engine.init();
    auto t = engine.makeTransaction();
    t->processUri(uri_, method_, version_);
    t->processRequestHeaders(request_header_find_, request_header_traversal_,
                             request_headers_.size(), nullptr);

    Variable::MatchedVarsNames matched_var("", false, false, "");
    result.clear();
    matched_var.evaluate(*t, result);
    EXPECT_EQ(result.size(), 4);

    EXPECT_EQ(std::get<std::string_view>(result[0].variant_), "ARGS:p1");
    EXPECT_EQ(std::get<std::string_view>(result[1].variant_), "ARGS:p2");
    EXPECT_EQ(std::get<std::string_view>(result[2].variant_), "ARGS:p3");
    EXPECT_EQ(std::get<std::string_view>(result[3].variant_), "ARGS:p4");
  }
}

TEST_F(VariableTest, MATCHED_VARS) {
  Common::EvaluateResults result;
  {
    const std::string directive = R"(
    SecRuleEngine On
    SecRule ARGS "@unconditionalMatch" "id:200,phase:1,chain"
      SecRule MATCHED_VARS "@unconditionalMatch" "t:none"
  )";

    Engine engine(spdlog::level::off);
    engine.load(directive);
    engine.init();
    auto t = engine.makeTransaction();
    t->processUri(uri_, method_, version_);
    t->processRequestHeaders(request_header_find_, request_header_traversal_,
                             request_headers_.size(), nullptr);

    Variable::MatchedVars matched_var("", false, false, "");
    result.clear();
    matched_var.evaluate(*t, result);
    EXPECT_EQ(result.size(), 4);

    EXPECT_EQ(std::get<std::string_view>(result[0].variant_), "v1");
    EXPECT_EQ(std::get<std::string_view>(result[1].variant_), "v2");
    EXPECT_EQ(std::get<std::string_view>(result[2].variant_), "v3");
    EXPECT_EQ(std::get<std::string_view>(result[3].variant_), "v4");
  }
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
  Common::EvaluateResults result;
  {
    const std::string directive = R"(
    SecRuleEngine On
    SecAction "id:100,phase:1,ctl:requestBodyProcessor=MULTIPART"
  )";

    std::string multipart_body = "----helloworld\r\n"
                                 "content-disposition: form-data; name=foo1\r\n"
                                 "header1: value1\r\n"
                                 "\r\n"
                                 "bar1\r\n"
                                 "----helloworld\r\n"
                                 "content-disposition: form-data; name=foo2\r\n"
                                 "header2: value2\r\n"
                                 "\r\n"
                                 "bar2\r\n"
                                 "----helloworld\r\n"
                                 "content-disposition: form-data; name=foo3\r\n"
                                 "header2: value3\r\n"
                                 "\r\n"
                                 "bar3\r\n"
                                 "----helloworld\r\n"
                                 "content-disposition: form-data; name=file1; filename=hello1\r\n"
                                 "\r\n"
                                 "world\r\n"
                                 "----helloworld\r\n"
                                 "content-disposition: form-data; name=file2; filename=hello2\r\n"
                                 "\r\n"
                                 "world\r\n"
                                 "----helloworld\r\n"
                                 "content-disposition: form-data; name=file3; filename=hello3\r\n"
                                 "\r\n"
                                 "world\r\n"
                                 "----helloworld--";

    request_headers_.emplace("content-type", R"(multipart/form-data; boundary=--helloworld)");
    Engine engine(spdlog::level::off);
    engine.load(directive);
    engine.init();
    auto t = engine.makeTransaction();

    t->processUri(uri_, "POST", version_);
    t->processRequestHeaders(request_header_find_, request_header_traversal_,
                             request_headers_.size(), nullptr);
    t->processRequestBody(multipart_body);

    Variable::MultipartFileName all("", false, false, "");
    result.clear();
    all.evaluate(*t, result);
    EXPECT_EQ(result.size(), 3);

    EXPECT_EQ(std::get<std::string_view>(result[0].variant_), "hello1");
    EXPECT_EQ(std::get<std::string_view>(result[1].variant_), "hello2");
    EXPECT_EQ(std::get<std::string_view>(result[2].variant_), "hello3");

    Variable::MultipartFileName all_count("", false, true, "");
    result.clear();
    all_count.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 3);

    Variable::MultipartFileName sub("file1", false, false, "");
    result.clear();
    sub.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<std::string_view>(result.front().variant_), "hello1");

    Variable::MultipartFileName sub_count("file1", false, true, "");
    result.clear();
    sub_count.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 1);

    {
      Variable::MultipartFileName sub_regex("/^file/", false, false, "");
      result.clear();
      sub_regex.evaluate(*t, result);
      EXPECT_EQ(result.size(), 3);
    }

    {
      Variable::MultipartFileName sub_regex("/^file\\d/", false, false, "");
      result.clear();
      sub_regex.evaluate(*t, result);
      EXPECT_EQ(result.size(), 3);
    }

    {
      Variable::MultipartFileName sub_regex("/^fa/", false, false, "");
      result.clear();
      sub_regex.evaluate(*t, result);
      EXPECT_EQ(result.size(), 0);
    }

    request_headers_.erase("content-type");
  }
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
  Common::EvaluateResults result;
  {
    const std::string directive = R"(
    SecRuleEngine On
    SecAction "id:100,phase:1,ctl:requestBodyProcessor=MULTIPART"
  )";

    std::string multipart_body = "----helloworld\r\n"
                                 "content-disposition: form-data; name=foo1\r\n"
                                 "header1: value1\r\n"
                                 "\r\n"
                                 "bar1\r\n"
                                 "----helloworld\r\n"
                                 "content-disposition: form-data; name=foo2\r\n"
                                 "header2: value2\r\n"
                                 "\r\n"
                                 "bar2\r\n"
                                 "----helloworld\r\n"
                                 "content-disposition: form-data; name=foo3\r\n"
                                 "header2: value3\r\n"
                                 "\r\n"
                                 "bar3\r\n"
                                 "----helloworld\r\n"
                                 "content-disposition: form-data; name=file1; filename=hello1\r\n"
                                 "\r\n"
                                 "world\r\n"
                                 "----helloworld\r\n"
                                 "content-disposition: form-data; name=file2; filename=hello2\r\n"
                                 "\r\n"
                                 "world\r\n"
                                 "----helloworld\r\n"
                                 "content-disposition: form-data; name=file3; filename=hello3\r\n"
                                 "\r\n"
                                 "world\r\n"
                                 "----helloworld--";

    request_headers_.emplace("content-type", R"(multipart/form-data; boundary=--helloworld)");
    Engine engine(spdlog::level::off);
    engine.load(directive);
    engine.init();
    auto t = engine.makeTransaction();

    t->processUri(uri_, "POST", version_);
    t->processRequestHeaders(request_header_find_, request_header_traversal_,
                             request_headers_.size(), nullptr);
    t->processRequestBody(multipart_body);

    Variable::MultipartName all("", false, false, "");
    result.clear();
    all.evaluate(*t, result);
    EXPECT_EQ(result.size(), 3);

    EXPECT_EQ(std::get<std::string_view>(result[0].variant_), "file1");
    EXPECT_EQ(std::get<std::string_view>(result[1].variant_), "file2");
    EXPECT_EQ(std::get<std::string_view>(result[2].variant_), "file3");

    Variable::MultipartName all_count("", false, true, "");
    result.clear();
    all_count.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 3);

    Variable::MultipartName sub("file1", false, false, "");
    result.clear();
    sub.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<std::string_view>(result.front().variant_), "file1");

    Variable::MultipartName sub_count("file1", false, true, "");
    result.clear();
    sub_count.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 1);

    {
      Variable::MultipartName sub_regex("/^file/", false, false, "");
      result.clear();
      sub_regex.evaluate(*t, result);
      EXPECT_EQ(result.size(), 3);
    }

    {
      Variable::MultipartName sub_regex("/^file\\d/", false, false, "");
      result.clear();
      sub_regex.evaluate(*t, result);
      EXPECT_EQ(result.size(), 3);
    }

    {
      Variable::MultipartName sub_regex("/^fa/", false, false, "");
      result.clear();
      sub_regex.evaluate(*t, result);
      EXPECT_EQ(result.size(), 0);
    }

    request_headers_.erase("content-type");
  }
}

TEST_F(VariableTest, MULTIPART_PART_HEADERS) {
  Common::EvaluateResults result;
  {
    const std::string directive = R"(
    SecRuleEngine On
    SecAction "id:100,phase:1,ctl:requestBodyProcessor=MULTIPART"
  )";

    std::string multipart_body = "----helloworld\r\n"
                                 "content-disposition: form-data; name=_charset_\r\n"
                                 "\r\n"
                                 "charset\r\n"
                                 "----helloworld\r\n"
                                 "content-disposition: form-data; name=foo1\r\n"
                                 "header1: value1\r\n"
                                 "\r\n"
                                 "bar1\r\n"
                                 "----helloworld\r\n"
                                 "content-disposition: form-data; name=foo2\r\n"
                                 "header2: value2\r\n"
                                 "\r\n"
                                 "bar2\r\n\n"
                                 "----helloworld\r\n"
                                 "content-disposition: form-data; name=foo3\r\n"
                                 "header2: value3\r\n"
                                 "\r\n"
                                 "bar3\r\n"
                                 "----helloworld--";

    request_headers_.emplace("content-type", R"(multipart/form-data; boundary=--helloworld)");
    Engine engine(spdlog::level::off);
    engine.load(directive);
    engine.init();
    auto t = engine.makeTransaction();

    t->processUri(uri_, "POST", version_);
    t->processRequestHeaders(request_header_find_, request_header_traversal_,
                             request_headers_.size(), nullptr);
    t->processRequestBody(multipart_body);

    // Test collection - get all headers
    Variable::MultipartPartHeaders all("", false, false, "");
    result.clear();
    all.evaluate(*t, result);
    EXPECT_EQ(result.size(), 3);

    // Test collection counter - count all headers
    Variable::MultipartPartHeaders all_count("", false, true, "");
    result.clear();
    all_count.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 3);

    // Test specific header by literal name
    Variable::MultipartPartHeaders sub("header1", false, false, "");
    result.clear();
    sub.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<std::string_view>(result.front().variant_), "header1: value1");

    // Test specific header counter
    Variable::MultipartPartHeaders sub_count("header1", false, true, "");
    result.clear();
    sub_count.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 1);

    // Test header that appears multiple times
    Variable::MultipartPartHeaders sub_multi("header2", false, false, "");
    result.clear();
    sub_multi.evaluate(*t, result);
    EXPECT_EQ(result.size(), 2);
    EXPECT_EQ(std::get<std::string_view>(result[0].variant_), "header2: value2");
    EXPECT_EQ(std::get<std::string_view>(result[1].variant_), "header2: value3");

    // Test regex matching
    {
      Variable::MultipartPartHeaders sub_regex("/^header/", false, false, "");
      result.clear();
      sub_regex.evaluate(*t, result);
      EXPECT_EQ(result.size(), 3);
    }

    // Test regex matching - specific pattern
    {
      Variable::MultipartPartHeaders sub_regex("/^header1/", false, false, "");
      result.clear();
      sub_regex.evaluate(*t, result);
      EXPECT_EQ(result.size(), 1);
    }

    // Test regex matching - no match
    {
      Variable::MultipartPartHeaders sub_regex("/^notexist/", false, false, "");
      result.clear();
      sub_regex.evaluate(*t, result);
      EXPECT_EQ(result.size(), 0);
    }

    {
      Variable::MultipartPartHeaders sub_regex("_charset_", false, false, "");
      result.clear();
      sub_regex.evaluate(*t, result);
      EXPECT_EQ(result.size(), 1);
      EXPECT_EQ(std::get<std::string_view>(result[0].variant_), "charset\r\n");
    }

    request_headers_.erase("content-type");
  }
}

TEST_F(VariableTest, MULTIPART_STRICT_ERROR) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, MULTIPART_UNMATCHED_BOUNDARY) {
  // TODO(zhouyu 2025-03-27): add the test cast
}

TEST_F(VariableTest, OUTBOUND_DATA_ERROR) {
  Common::EvaluateResults result;

  // Test response body size below limit
  {
    const std::string directive = R"(
    SecRuleEngine On
    SecResponseBodyLimit 1024
  )";

    std::string response_body_below = "short response";
    Engine engine(spdlog::level::off);
    engine.load(directive);
    engine.init();
    auto t = engine.makeTransaction();
    t->processUri(uri_, "GET", version_);
    t->processRequestHeaders(request_header_find_, request_header_traversal_,
                             request_headers_.size(), nullptr);
    t->processResponseHeaders("200", "HTTP/1.1", response_header_find_, response_header_traversal_,
                              response_headers_.size(), nullptr);
    t->processResponseBody(response_body_below, nullptr);

    Variable::OutboundDataError outbound_data_error("", false, false, "");
    result.clear();
    outbound_data_error.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 0);
  }

  // Test response body size above limit
  {
    const std::string directive = R"(
    SecRuleEngine On
    SecResponseBodyLimit 10
  )";

    std::string response_body_above = "this is a very long response body that exceeds the limit";
    Engine engine(spdlog::level::off);
    engine.load(directive);
    engine.init();
    auto t = engine.makeTransaction();
    t->processUri(uri_, "GET", version_);
    t->processRequestHeaders(request_header_find_, request_header_traversal_,
                             request_headers_.size(), nullptr);
    t->processResponseHeaders("200", "HTTP/1.1", response_header_find_, response_header_traversal_,
                              response_headers_.size(), nullptr);
    t->processResponseBody(response_body_above, nullptr);

    Variable::OutboundDataError outbound_data_error("", false, false, "");
    result.clear();
    outbound_data_error.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 1);
  }

  // Test counter mode
  {
    const std::string directive = R"(
    SecRuleEngine On
    SecResponseBodyLimit 10
  )";

    std::string response_body_above = "this is a very long response body";
    Engine engine(spdlog::level::off);
    engine.load(directive);
    engine.init();
    auto t = engine.makeTransaction();
    t->processUri(uri_, "GET", version_);
    t->processRequestHeaders(request_header_find_, request_header_traversal_,
                             request_headers_.size(), nullptr);
    t->processResponseHeaders("200", "HTTP/1.1", response_header_find_, response_header_traversal_,
                              response_headers_.size(), nullptr);
    t->processResponseBody(response_body_above, nullptr);

    Variable::OutboundDataError outbound_data_error_count("", false, true, "");
    result.clear();
    outbound_data_error_count.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 1);
  }
}

TEST_F(VariableTest, PATH_INFO) {
  Common::EvaluateResults result;

  // Test collection mode - should return relative URI path
  {
    Variable::PathInfo path_info("", false, false, "");
    result.clear();
    path_info.evaluate(*t_, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<std::string_view>(result.front().variant_), "/");
  }

  // Test counter mode - should return 1 (path exists)
  {
    Variable::PathInfo path_info_count("", false, true, "");
    result.clear();
    path_info_count.evaluate(*t_, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 1);
  }

  // Test with empty path
  {
    Engine engine(spdlog::level::off);
    engine.init();
    auto t = engine.makeTransaction();
    t->processUri("", "GET", "1.1");

    Variable::PathInfo path_info("", false, false, "");
    result.clear();
    path_info.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<std::string_view>(result.front().variant_), "");

    Variable::PathInfo path_info_count("", false, true, "");
    result.clear();
    path_info_count.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 0);
  }

  // Test with path containing no query string
  {
    Engine engine(spdlog::level::off);
    engine.init();
    auto t = engine.makeTransaction();
    t->processUri("/path/to/resource", "GET", "1.1");

    Variable::PathInfo path_info("", false, false, "");
    result.clear();
    path_info.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<std::string_view>(result.front().variant_), "/path/to/resource");

    Variable::PathInfo path_info_count("", false, true, "");
    result.clear();
    path_info_count.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 1);
  }
}

TEST_F(VariableTest, QUERY_STRING) {
  Common::EvaluateResults result;

  // Test with default URI containing query string
  {
    Variable::QueryString query_string("", false, false, "");
    result.clear();
    query_string.evaluate(*t_, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<std::string_view>(result.front().variant_), "p1=v1&p2=v2&p3=v3&p4=v4");
  }

  // Test counter mode - should return 1 when query string exists
  {
    Variable::QueryString query_string_count("", false, true, "");
    result.clear();
    query_string_count.evaluate(*t_, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 1);
  }

  // Test with URI without query string
  {
    Engine new_engine(spdlog::level::off);
    new_engine.init();
    auto t = new_engine.makeTransaction();
    t->processUri("/path/without/query", "GET", "1.1");

    Variable::QueryString query_string("", false, false, "");
    result.clear();
    query_string.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<std::string_view>(result.front().variant_), "");
  }

  // Test counter mode with empty query string - should return 0
  {
    Engine new_engine(spdlog::level::off);
    new_engine.init();
    auto t = new_engine.makeTransaction();
    t->processUri("/path/without/query", "GET", "1.1");

    Variable::QueryString query_string_count("", false, true, "");
    result.clear();
    query_string_count.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 0);
  }
}

TEST_F(VariableTest, REMOTE_ADDR) {
  Common::EvaluateResults result;

  // Test getting remote address
  {
    Variable::RemoteAddr remote_addr("", false, false, "");
    result.clear();
    remote_addr.evaluate(*t_, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<std::string_view>(result.front().variant_), "192.168.1.100");
  }

  // Test counter mode - should return 1 (address is not empty)
  {
    Variable::RemoteAddr remote_addr_count("", false, true, "");
    result.clear();
    remote_addr_count.evaluate(*t_, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 1);
  }
}

TEST_F(VariableTest, REMOTE_HOST) {
  Common::EvaluateResults result;

  // Test getting remote host (same as remote address in this implementation)
  {
    Variable::RemoteHost remote_host("", false, false, "");
    result.clear();
    remote_host.evaluate(*t_, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<std::string_view>(result.front().variant_), "192.168.1.100");
  }

  // Test counter mode
  {
    Variable::RemoteHost remote_host_count("", false, true, "");
    result.clear();
    remote_host_count.evaluate(*t_, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 1);
  }
}

TEST_F(VariableTest, REMOTE_PORT) {
  Common::EvaluateResults result;

  // Test getting remote port
  {
    Variable::RemotePort remote_port("", false, false, "");
    result.clear();
    remote_port.evaluate(*t_, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 20000);
  }

  // Test counter mode - should return 1 (port is non-zero)
  {
    Variable::RemotePort remote_port_count("", false, true, "");
    result.clear();
    remote_port_count.evaluate(*t_, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 1);
  }
}

TEST_F(VariableTest, REMOTE_USER) {
  Common::EvaluateResults result;

  // Test without Authorization header - should return empty string
  {
    Variable::RemoteUser remote_user("", false, false, "");
    result.clear();
    remote_user.evaluate(*t_, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<std::string_view>(result.front().variant_), "");

    Variable::RemoteUser remote_user_count("", false, true, "");
    result.clear();
    remote_user_count.evaluate(*t_, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 0);
  }

  // Test with Basic Auth header - should return decoded username
  {
    const std::string directive = R"(
    SecRuleEngine On
    SecAction "id:100,phase:1"
  )";

    // Base64 encode "testuser:testpass"
    std::string auth_header = "Basic dGVzdHVzZXI6dGVzdHBhc3M=";

    Engine engine(spdlog::level::off);
    engine.load(directive);
    engine.init();
    auto t = engine.makeTransaction();
    t->processConnection(downstream_ip_, downstream_port_, upstream_ip_, upstream_port_);
    t->processUri(uri_, method_, version_);

    // Set Authorization header
    request_headers_.emplace("authorization", auth_header);

    t->processRequestHeaders(request_header_find_, request_header_traversal_,
                             request_headers_.size(), nullptr);

    Variable::RemoteUser remote_user("", false, false, "");
    result.clear();
    remote_user.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<std::string_view>(result.front().variant_), "testuser");

    Variable::RemoteUser remote_user_count("", false, true, "");
    result.clear();
    remote_user_count.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 1);

    request_headers_.erase("authorization");
  }
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
  Common::EvaluateResults result;

  // Test URLENCODED body processor
  {
    const std::string directive = R"(
    SecRuleEngine On
    SecAction "id:100,phase:1,ctl:requestBodyProcessor=URLENCODED"
  )";

    Engine engine(spdlog::level::off);
    engine.load(directive);
    engine.init();
    auto t = engine.makeTransaction();
    t->processUri(uri_, "POST", version_);
    t->processRequestHeaders(request_header_find_, request_header_traversal_,
                             request_headers_.size(), nullptr);

    Variable::ReqBodyProcessor reqbody_processor("", false, false, "");
    result.clear();
    reqbody_processor.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<std::string_view>(result.front().variant_), "URLENCODED");

    Variable::ReqBodyProcessor reqbody_processor_count("", false, true, "");
    result.clear();
    reqbody_processor_count.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 1);
  }

  // Test JSON body processor
  {
    const std::string directive = R"(
    SecRuleEngine On
    SecAction "id:100,phase:1,ctl:requestBodyProcessor=JSON"
  )";

    Engine engine(spdlog::level::off);
    engine.load(directive);
    engine.init();
    auto t = engine.makeTransaction();
    t->processUri(uri_, "POST", version_);
    t->processRequestHeaders(request_header_find_, request_header_traversal_,
                             request_headers_.size(), nullptr);

    Variable::ReqBodyProcessor reqbody_processor("", false, false, "");
    result.clear();
    reqbody_processor.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<std::string_view>(result.front().variant_), "JSON");
  }

  // Test MULTIPART body processor
  {
    const std::string directive = R"(
    SecRuleEngine On
    SecAction "id:100,phase:1,ctl:requestBodyProcessor=MULTIPART"
  )";

    Engine engine(spdlog::level::off);
    engine.load(directive);
    engine.init();
    auto t = engine.makeTransaction();
    t->processUri(uri_, "POST", version_);
    request_headers_.emplace("content-type", "multipart/form-data; boundary=--test");
    t->processRequestHeaders(request_header_find_, request_header_traversal_,
                             request_headers_.size(), nullptr);

    Variable::ReqBodyProcessor reqbody_processor("", false, false, "");
    result.clear();
    reqbody_processor.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<std::string_view>(result.front().variant_), "MULTIPART");

    request_headers_.erase("content-type");
  }

  // Test XML body processor
  {
    const std::string directive = R"(
    SecRuleEngine On
    SecAction "id:100,phase:1,ctl:requestBodyProcessor=XML"
  )";

    Engine engine(spdlog::level::off);
    engine.load(directive);
    engine.init();
    auto t = engine.makeTransaction();
    t->processUri(uri_, "POST", version_);
    t->processRequestHeaders(request_header_find_, request_header_traversal_,
                             request_headers_.size(), nullptr);

    Variable::ReqBodyProcessor reqbody_processor("", false, false, "");
    result.clear();
    reqbody_processor.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<std::string_view>(result.front().variant_), "XML");
  }
}

TEST_F(VariableTest, REQUEST_BASENAME) {
  Common::EvaluateResults result;

  // Test with default URI (root path)
  {
    Variable::RequestBaseName request_basename("", false, false, "");
    result.clear();
    request_basename.evaluate(*t_, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<std::string_view>(result.front().variant_), "");

    Variable::RequestBaseName request_basename_count("", false, true, "");
    result.clear();
    request_basename_count.evaluate(*t_, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 0);
  }

  // Test with URI containing file path
  {
    const std::string directive = R"(
    SecRuleEngine On
  )";

    Engine engine(spdlog::level::off);
    engine.load(directive);
    engine.init();
    auto t = engine.makeTransaction();
    t->processUri("/path/to/file.php?arg=value", "GET", version_);
    t->processRequestHeaders(request_header_find_, request_header_traversal_,
                             request_headers_.size(), nullptr);

    Variable::RequestBaseName request_basename("", false, false, "");
    result.clear();
    request_basename.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<std::string_view>(result.front().variant_), "file.php");
  }
}

TEST_F(VariableTest, REQUEST_BODY_LENGTH) {
  Common::EvaluateResults result;

  // Test with empty request body
  {
    Variable::RequestBodyLength body_length("", false, false, "");
    result.clear();
    body_length.evaluate(*t_, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 0);

    Variable::RequestBodyLength body_length_count("", false, true, "");
    result.clear();
    body_length_count.evaluate(*t_, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 0);
  }

  // Test with non-empty request body
  {
    const std::string directive = R"(
    SecRuleEngine On
    SecAction "id:100,phase:1,ctl:requestBodyProcessor=URLENCODED"
    )";

    std::string request_body = "p1=v1&p2=v2&p3=v3";
    Engine engine(spdlog::level::off);
    engine.load(directive);
    engine.init();
    auto t = engine.makeTransaction();
    t->processUri(uri_, "POST", version_);
    t->processRequestHeaders(request_header_find_, request_header_traversal_,
                             request_headers_.size(), nullptr);
    t->processRequestBody(request_body);

    Variable::RequestBodyLength body_length("", false, false, "");
    result.clear();
    body_length.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_),
              static_cast<int64_t>(request_body.size()));

    Variable::RequestBodyLength body_length_count("", false, true, "");
    result.clear();
    body_length_count.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 1);
  }
}

TEST_F(VariableTest, REQUEST_BODY) {
  Common::EvaluateResults result;

  // Test empty request body
  {
    Engine engine(spdlog::level::off);
    engine.init();
    auto t = engine.makeTransaction();
    t->processUri(uri_, "POST", version_);
    t->processRequestHeaders(request_header_find_, request_header_traversal_,
                             request_headers_.size(), nullptr);
    // No request body processed

    Variable::RequestBody request_body("", false, false, "");
    result.clear();
    request_body.evaluate(*t, result);
    EXPECT_EQ(result.size(), 0);

    Variable::RequestBody request_body_count("", false, true, "");
    result.clear();
    request_body_count.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 0);
  }

  // Test non-empty request body
  {
    const std::string directive = R"(
    SecRuleEngine On
    SecAction "id:100,phase:1,ctl:requestBodyProcessor=URLENCODED"
  )";

    std::string request_body_data = "p1=v1&p2=v2&p3=v3";
    Engine engine(spdlog::level::off);
    engine.load(directive);
    engine.init();
    auto t = engine.makeTransaction();
    t->processUri(uri_, "POST", version_);
    t->processRequestHeaders(request_header_find_, request_header_traversal_,
                             request_headers_.size(), nullptr);
    t->processRequestBody(request_body_data);

    Variable::RequestBody request_body("", false, false, "");
    result.clear();
    request_body.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<std::string_view>(result.front().variant_), request_body_data);

    Variable::RequestBody request_body_count("", false, true, "");
    result.clear();
    request_body_count.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 1);
  }
}

TEST_F(VariableTest, REQUEST_COOKIES_NAMES) {
  Common::EvaluateResults result;

  // Test with cookies in request headers
  {
    const std::string directive = R"(
    SecRuleEngine On
  )";

    Engine engine(spdlog::level::off);
    engine.load(directive);
    engine.init();
    auto t = engine.makeTransaction();
    t->processUri(uri_, "GET", version_);

    // Add Cookie header
    std::unordered_multimap<std::string, std::string> headers;
    headers.emplace("cookie", "session_id=abc123; user_id=xyz789; prefs=dark_mode");

    auto header_find = [&](const std::string& key) {
      std::vector<std::string_view> result;
      auto range = headers.equal_range(key);
      for (auto iter = range.first; iter != range.second; ++iter) {
        result.emplace_back(iter->second.data(), iter->second.length());
      }
      return result;
    };

    auto header_traversal = [&](HeaderTraversalCallback callback) {
      for (auto& [key, value] : headers) {
        if (!callback(key, value)) {
          break;
        }
      }
    };

    t->processRequestHeaders(header_find, header_traversal, headers.size(), nullptr);

    Variable::RequestCookiesNames all("", false, false, "");
    result.clear();
    all.evaluate(*t, result);
    EXPECT_EQ(result.size(), 3);

    Variable::RequestCookiesNames all_count("", false, true, "");
    result.clear();
    all_count.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 3);

    Variable::RequestCookiesNames sub("session_id", false, false, "");
    result.clear();
    sub.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<std::string_view>(result.front().variant_), "session_id");

    Variable::RequestCookiesNames sub_count("session_id", false, true, "");
    result.clear();
    sub_count.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 1);

    {
      Variable::RequestCookiesNames sub_regex("/^user/", false, false, "");
      result.clear();
      sub_regex.evaluate(*t, result);
      EXPECT_EQ(result.size(), 1);
      EXPECT_EQ(std::get<std::string_view>(result.front().variant_), "user_id");
    }
  }
}

TEST_F(VariableTest, REQUEST_COOKIES) {
  Common::EvaluateResults result;

  // Test with cookies in request headers
  {
    const std::string directive = R"(
    SecRuleEngine On
  )";

    std::unordered_multimap<std::string, std::string> headers;
    headers.emplace("cookie", "cookie1=value1; cookie2=value2; session=abc123");

    auto header_find = [&](const std::string& key) {
      std::vector<std::string_view> result;
      auto range = headers.equal_range(key);
      for (auto iter = range.first; iter != range.second; ++iter) {
        result.emplace_back(iter->second.data(), iter->second.length());
      }
      return result;
    };

    auto header_traversal = [&](HeaderTraversalCallback callback) {
      for (auto& [key, value] : headers) {
        if (!callback(key, value)) {
          break;
        }
      }
    };

    Engine engine(spdlog::level::off);
    engine.load(directive);
    engine.init();
    auto t = engine.makeTransaction();
    t->processUri(uri_, "GET", version_);
    t->processRequestHeaders(header_find, header_traversal, headers.size(), nullptr);

    Variable::RequestCookies all("", false, false, "");
    result.clear();
    all.evaluate(*t, result);
    EXPECT_EQ(result.size(), 3);

    Variable::RequestCookies all_count("", false, true, "");
    result.clear();
    all_count.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 3);

    Variable::RequestCookies sub("cookie1", false, false, "");
    result.clear();
    sub.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<std::string_view>(result.front().variant_), "value1");

    Variable::RequestCookies sub_count("cookie2", false, true, "");
    result.clear();
    sub_count.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 1);

    {
      Variable::RequestCookies sub_regex("/^cookie/", false, false, "");
      result.clear();
      sub_regex.evaluate(*t, result);
      EXPECT_EQ(result.size(), 2);
    }
  }
}

TEST_F(VariableTest, REQUEST_FILENAME) {
  Common::EvaluateResults result;

  // Test with default URI ("/")
  {
    Variable::RequestFileName request_filename("", false, false, "");
    result.clear();
    request_filename.evaluate(*t_, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<std::string_view>(result.front().variant_), "/");

    Variable::RequestFileName request_filename_count("", false, true, "");
    result.clear();
    request_filename_count.evaluate(*t_, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 1);
  }

  // Test with path URI
  {
    Engine engine(spdlog::level::off);
    engine.init();
    auto t = engine.makeTransaction();
    t->processConnection(downstream_ip_, downstream_port_, upstream_ip_, upstream_port_);
    t->processUri("/path/to/resource?query=value", "GET", version_);
    t->processRequestHeaders(request_header_find_, request_header_traversal_,
                             request_headers_.size(), nullptr);

    Variable::RequestFileName request_filename("", false, false, "");
    result.clear();
    request_filename.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<std::string_view>(result.front().variant_), "/path/to/resource");
  }
}

TEST_F(VariableTest, REQUEST_HEADERS_NAMES) {
  Common::EvaluateResults result;

  // Test with default headers from SetUp
  {
    Variable::RequestHeadersNames all("", false, false, "");
    result.clear();
    all.evaluate(*t_, result);
    EXPECT_EQ(result.size(), 6);
  }

  // Test counter mode - should return total header count
  {
    Variable::RequestHeadersNames all_count("", false, true, "");
    result.clear();
    all_count.evaluate(*t_, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 6);
  }

  // Test specific header name
  {
    Variable::RequestHeadersNames header("host", false, false, "");
    result.clear();
    header.evaluate(*t_, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<std::string_view>(result.front().variant_), "host");
  }

  // Test specific header counter
  {
    Variable::RequestHeadersNames header_count("host", false, true, "");
    result.clear();
    header_count.evaluate(*t_, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 1);
  }

  // Test regex match for header names
  {
    Variable::RequestHeadersNames header_regex("/^h/", false, false, "");
    result.clear();
    header_regex.evaluate(*t_, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<std::string_view>(result.front().variant_), "host");
  }
}

TEST_F(VariableTest, REQUEST_HEADERS) {
  Common::EvaluateResults result;

  // Test getting all request header values
  {
    Variable::RequestHeaders all("", false, false, "");
    result.clear();
    all.evaluate(*t_, result);
    EXPECT_EQ(result.size(), 6);
  }

  // Test counting all request headers
  {
    Variable::RequestHeaders all_count("", false, true, "");
    result.clear();
    all_count.evaluate(*t_, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 6);
  }

  // Test getting specific header value
  {
    Variable::RequestHeaders host("host", false, false, "");
    result.clear();
    host.evaluate(*t_, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<std::string_view>(result.front().variant_), "localhost:80");
  }

  // Test getting cookie header (multiple values)
  {
    Variable::RequestHeaders cookie("cookie", false, false, "");
    result.clear();
    cookie.evaluate(*t_, result);
    EXPECT_EQ(result.size(), 2);
  }

  // Test counting specific header
  {
    Variable::RequestHeaders host_count("host", false, true, "");
    result.clear();
    host_count.evaluate(*t_, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 1);
  }

  // Test counting cookie header (multiple values)
  {
    Variable::RequestHeaders cookie_count("cookie", false, true, "");
    result.clear();
    cookie_count.evaluate(*t_, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 2);
  }
}

TEST_F(VariableTest, REQUEST_LINE) {
  Common::EvaluateResults result;

  // Test getting request line
  {
    Variable::RequestLine request_line("", false, false, "");
    result.clear();
    request_line.evaluate(*t_, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<std::string_view>(result.front().variant_),
              "GET /?p1=v1&p2=v2&p3=v3&p4=v4 HTTP/1.1");
  }

  // Test counter mode
  {
    Variable::RequestLine request_line_count("", false, true, "");
    result.clear();
    request_line_count.evaluate(*t_, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 1);
  }
}

TEST_F(VariableTest, REQUEST_METHOD) {
  Common::EvaluateResults result;

  // Test getting request method (default is GET)
  {
    Variable::RequestMothod request_method("", false, false, "");
    result.clear();
    request_method.evaluate(*t_, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<std::string_view>(result.front().variant_), "GET");
  }

  // Test counter mode - should return 1 (method is not empty)
  {
    Variable::RequestMothod request_method_count("", false, true, "");
    result.clear();
    request_method_count.evaluate(*t_, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 1);
  }

  // Test with POST method
  {
    Engine engine(spdlog::level::off);
    engine.init();
    auto t = engine.makeTransaction();
    t->processConnection(downstream_ip_, downstream_port_, upstream_ip_, upstream_port_);
    t->processUri(uri_, "POST", version_);
    t->processRequestHeaders(request_header_find_, request_header_traversal_,
                             request_headers_.size(), nullptr);

    Variable::RequestMothod request_method("", false, false, "");
    result.clear();
    request_method.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<std::string_view>(result.front().variant_), "POST");
  }
}

TEST_F(VariableTest, REQUEST_PROTOCOL) {
  Common::EvaluateResults result;

  // Test getting request protocol (default is HTTP/1.1)
  {
    Variable::RequestProtocol request_protocol("", false, false, "");
    result.clear();
    request_protocol.evaluate(*t_, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<std::string_view>(result.front().variant_), "HTTP/1.1");
  }

  // Test counter mode
  {
    Variable::RequestProtocol request_protocol_count("", false, true, "");
    result.clear();
    request_protocol_count.evaluate(*t_, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 1);
  }
}

TEST_F(VariableTest, REQUEST_URI_RAW) {
  Common::EvaluateResults result;

  // Test getting raw URI
  {
    Variable::RequestUriRaw request_uri_raw("", false, false, "");
    result.clear();
    request_uri_raw.evaluate(*t_, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<std::string_view>(result.front().variant_), "/?p1=v1&p2=v2&p3=v3&p4=v4");
  }

  // Test counter mode
  {
    Variable::RequestUriRaw request_uri_raw_count("", false, true, "");
    result.clear();
    request_uri_raw_count.evaluate(*t_, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 1);
  }
}

TEST_F(VariableTest, REQUEST_URI) {
  Common::EvaluateResults result;

  // Test with default URI (contains query string)
  {
    Variable::RequestUri request_uri("", false, false, "");
    result.clear();
    request_uri.evaluate(*t_, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<std::string_view>(result.front().variant_), "/?p1=v1&p2=v2&p3=v3&p4=v4");
  }

  // Test counter mode
  {
    Variable::RequestUri request_uri_count("", false, true, "");
    result.clear();
    request_uri_count.evaluate(*t_, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 1);
  }

  // Test with URL-encoded URI
  {
    Engine engine(spdlog::level::off);
    engine.init();
    auto t = engine.makeTransaction();
    t->processConnection(downstream_ip_, downstream_port_, upstream_ip_, upstream_port_);
    // URL-encoded: /path%20with%20spaces?key=%3Dvalue%26
    // Decoded: /path with spaces?key==value&
    t->processUri("/path%20with%20spaces?key=%3Dvalue%26", "GET", "1.1");
    t->processRequestHeaders(request_header_find_, request_header_traversal_,
                             request_headers_.size(), nullptr);

    Variable::RequestUri request_uri("", false, false, "");
    result.clear();
    request_uri.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    // uri_ should be URL-decoded
    EXPECT_EQ(std::get<std::string_view>(result.front().variant_), "/path with spaces?key==value&");

    // Compare with REQUEST_URI_RAW (should not be decoded)
    Variable::RequestUriRaw request_uri_raw("", false, false, "");
    result.clear();
    request_uri_raw.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<std::string_view>(result.front().variant_),
              "/path%20with%20spaces?key=%3Dvalue%26");
  }
}

TEST_F(VariableTest, RESPONSE_BODY) {
  Common::EvaluateResults result;

  // Test with empty response body
  {
    Variable::ResponseBody response_body("", false, false, "");
    result.clear();
    response_body.evaluate(*t_, result);
    EXPECT_EQ(result.size(), 0);

    Variable::ResponseBody response_body_count("", false, true, "");
    result.clear();
    response_body_count.evaluate(*t_, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 0);
  }

  // Test with non-empty response body
  {
    const std::string directive = R"(
    SecRuleEngine On
  )";

    Engine engine(spdlog::level::off);
    engine.load(directive);
    engine.init();
    auto t = engine.makeTransaction();
    t->processUri(uri_, "GET", version_);
    t->processRequestHeaders(request_header_find_, request_header_traversal_,
                             request_headers_.size(), nullptr);
    t->processResponseHeaders("200", "HTTP/1.1", response_header_find_, response_header_traversal_,
                              response_headers_.size(), nullptr);
    t->processResponseBody("response body content", nullptr);

    Variable::ResponseBody response_body("", false, false, "");
    result.clear();
    response_body.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<std::string_view>(result.front().variant_), "response body content");

    Variable::ResponseBody response_body_count("", false, true, "");
    result.clear();
    response_body_count.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 1);
  }
}

TEST_F(VariableTest, RESPONSE_CONTENT_LENGTH) {
  Common::EvaluateResults result;

  // Test with content-length header
  {
    response_headers_.emplace("content-length", "1024");

    Engine engine(spdlog::level::off);
    engine.init();
    auto t = engine.makeTransaction();
    t->processUri(uri_, "GET", version_);
    t->processRequestHeaders(request_header_find_, request_header_traversal_,
                             request_headers_.size(), nullptr);
    t->processResponseHeaders("200", "HTTP/1.1", response_header_find_, response_header_traversal_,
                              response_headers_.size(), nullptr);

    Variable::ResponseContentLength response_content_length("", false, false, "");
    result.clear();
    response_content_length.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<std::string_view>(result.front().variant_), "1024");

    Variable::ResponseContentLength response_content_length_count("", false, true, "");
    result.clear();
    response_content_length_count.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 1);

    response_headers_.erase("content-length");
  }

  // Test without content-length header
  {
    Engine engine(spdlog::level::off);
    engine.init();
    auto t = engine.makeTransaction();
    t->processUri(uri_, "GET", version_);
    t->processRequestHeaders(request_header_find_, request_header_traversal_,
                             request_headers_.size(), nullptr);
    t->processResponseHeaders("200", "HTTP/1.1", response_header_find_, response_header_traversal_,
                              response_headers_.size(), nullptr);

    Variable::ResponseContentLength response_content_length("", false, false, "");
    result.clear();
    response_content_length.evaluate(*t, result);
    EXPECT_EQ(result.size(), 0);

    Variable::ResponseContentLength response_content_length_count("", false, true, "");
    result.clear();
    response_content_length_count.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 0);
  }
}

TEST_F(VariableTest, RESPONSE_CONTENT_TYPE) {
  Common::EvaluateResults result;

  // Test without Content-Type header
  {
    Engine engine(spdlog::level::off);
    engine.init();
    auto t = engine.makeTransaction();
    t->processUri(uri_, "GET", version_);
    t->processRequestHeaders(request_header_find_, request_header_traversal_,
                             request_headers_.size(), nullptr);
    t->processResponseHeaders("200", "HTTP/1.1", response_header_find_, response_header_traversal_,
                              response_headers_.size(), nullptr);

    Variable::ResponseContentType response_content_type("", false, false, "");
    result.clear();
    response_content_type.evaluate(*t, result);
    EXPECT_EQ(result.size(), 0);

    Variable::ResponseContentType response_content_type_count("", false, true, "");
    result.clear();
    response_content_type_count.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 0);
  }

  // Test with Content-Type header
  {
    response_headers_.emplace("content-type", "application/json");
    Engine engine(spdlog::level::off);
    engine.init();
    auto t = engine.makeTransaction();
    t->processUri(uri_, "GET", version_);
    t->processRequestHeaders(request_header_find_, request_header_traversal_,
                             request_headers_.size(), nullptr);
    t->processResponseHeaders("200", "HTTP/1.1", response_header_find_, response_header_traversal_,
                              response_headers_.size(), nullptr);

    Variable::ResponseContentType response_content_type("", false, false, "");
    result.clear();
    response_content_type.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<std::string_view>(result.front().variant_), "application/json");

    Variable::ResponseContentType response_content_type_count("", false, true, "");
    result.clear();
    response_content_type_count.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 1);

    response_headers_.erase("content-type");
  }
}

TEST_F(VariableTest, RESPONSE_HEADERS_NAMES) {
  Common::EvaluateResults result;

  // Test with response headers
  {
    // Set up response headers
    std::unordered_multimap<std::string, std::string> response_headers{
        {"content-type", "text/html"}, {"content-length", "100"}, {"server", "nginx"}};

    HeaderFind response_header_find = [&](const std::string& key) {
      std::vector<std::string_view> result;
      auto range = response_headers.equal_range(key);
      for (auto iter = range.first; iter != range.second; ++iter) {
        result.emplace_back(iter->second.data(), iter->second.length());
      }
      return result;
    };

    HeaderTraversal response_header_traversal = [&](HeaderTraversalCallback callback) {
      for (auto& [key, value] : response_headers) {
        if (!callback(key, value)) {
          break;
        }
      }
    };

    Engine engine(spdlog::level::off);
    engine.init();
    auto t = engine.makeTransaction();
    t->processUri(uri_, method_, version_);
    t->processRequestHeaders(request_header_find_, request_header_traversal_,
                             request_headers_.size(), nullptr);
    t->processResponseHeaders("200", "HTTP/1.1", response_header_find, response_header_traversal,
                              response_headers.size(), nullptr);

    Variable::ResponseHeadersNames all("", false, false, "");
    result.clear();
    all.evaluate(*t, result);
    EXPECT_EQ(result.size(), 3);

    Variable::ResponseHeadersNames all_count("", false, true, "");
    result.clear();
    all_count.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 3);

    Variable::ResponseHeadersNames sub("content-type", false, false, "");
    result.clear();
    sub.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<std::string_view>(result.front().variant_), "content-type");

    Variable::ResponseHeadersNames sub_count("content-type", false, true, "");
    result.clear();
    sub_count.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 1);

    {
      Variable::ResponseHeadersNames sub_regex("/^content/", false, false, "");
      result.clear();
      sub_regex.evaluate(*t, result);
      EXPECT_EQ(result.size(), 2);
    }
  }
}

TEST_F(VariableTest, RESPONSE_HEADERS) {
  Common::EvaluateResults result;

  // Test with response headers
  {
    const std::string directive = R"(
    SecRuleEngine On
  )";

    Engine engine(spdlog::level::off);
    engine.load(directive);
    engine.init();
    auto t = engine.makeTransaction();
    t->processUri(uri_, "GET", version_);
    t->processRequestHeaders(request_header_find_, request_header_traversal_,
                             request_headers_.size(), nullptr);

    // Set up response headers
    std::unordered_multimap<std::string, std::string> response_headers{
        {"content-type", "text/html"}, {"content-length", "100"}, {"server", "nginx"}};

    HeaderFind response_header_find = [&](const std::string& key) {
      std::vector<std::string_view> result;
      auto range = response_headers.equal_range(key);
      for (auto iter = range.first; iter != range.second; ++iter) {
        result.emplace_back(iter->second.data(), iter->second.length());
      }
      return result;
    };

    HeaderTraversal response_header_traversal = [&](HeaderTraversalCallback callback) {
      for (auto& [key, value] : response_headers) {
        if (!callback(key, value)) {
          break;
        }
      }
    };

    t->processResponseHeaders("200", "HTTP/1.1", response_header_find, response_header_traversal,
                              response_headers.size(), nullptr);

    // Test get all response header values
    Variable::ResponseHeaders all("", false, false, "");
    result.clear();
    all.evaluate(*t, result);
    EXPECT_EQ(result.size(), 3);

    // Test count all response headers
    Variable::ResponseHeaders all_count("", false, true, "");
    result.clear();
    all_count.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 3);

    // Test get specific response header value
    Variable::ResponseHeaders content_type("content-type", false, false, "");
    result.clear();
    content_type.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<std::string_view>(result.front().variant_), "text/html");

    // Test count specific response header
    Variable::ResponseHeaders content_type_count("content-type", false, true, "");
    result.clear();
    content_type_count.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 1);

    // Test regex match response header names
    {
      Variable::ResponseHeaders content_regex("/^content/", false, false, "");
      result.clear();
      content_regex.evaluate(*t, result);
      EXPECT_EQ(result.size(), 2);
    }
  }
}

TEST_F(VariableTest, RESPONSE_PROTOCOL) {
  Common::EvaluateResults result;

  // Test with response headers processed
  {
    Engine engine(spdlog::level::off);
    engine.init();
    auto t = engine.makeTransaction();
    t->processUri(uri_, method_, version_);
    t->processRequestHeaders(request_header_find_, request_header_traversal_,
                             request_headers_.size(), nullptr);
    t->processResponseHeaders("200", "HTTP/1.1", response_header_find_, response_header_traversal_,
                              response_headers_.size(), nullptr);

    Variable::ResponseProtocol response_protocol("", false, false, "");
    result.clear();
    response_protocol.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<std::string_view>(result.front().variant_), "HTTP/1.1");

    Variable::ResponseProtocol response_protocol_count("", false, true, "");
    result.clear();
    response_protocol_count.evaluate(*t, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 1);
  }
}

TEST_F(VariableTest, RESPONSE_STATUS) {
  Common::EvaluateResults result;

  // Test with response headers processed
  t_->processResponseHeaders("200", "HTTP/1.1", response_header_find_, response_header_traversal_,
                             response_headers_.size(), nullptr);
  {
    Variable::ResponseStatus response_status("", false, false, "");
    result.clear();
    response_status.evaluate(*t_, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<std::string_view>(result.front().variant_), "200");

    Variable::ResponseStatus response_status_count("", false, true, "");
    result.clear();
    response_status_count.evaluate(*t_, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 1);
  }
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
  Common::EvaluateResults result;

  // Test getting server address
  {
    Variable::ServerAddr server_addr("", false, false, "");
    result.clear();
    server_addr.evaluate(*t_, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<std::string_view>(result.front().variant_), "192.168.1.200");
  }

  // Test counter mode
  {
    Variable::ServerAddr server_addr_count("", false, true, "");
    result.clear();
    server_addr_count.evaluate(*t_, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 1);
  }
}

TEST_F(VariableTest, SERVER_NAME) {
  Common::EvaluateResults result;

  // Test using default host header from SetUp
  {
    Variable::ServerName server_name("", false, false, "");
    result.clear();
    server_name.evaluate(*t_, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<std::string_view>(result.front().variant_), "localhost:80");

    Variable::ServerName server_name_count("", false, true, "");
    result.clear();
    server_name_count.evaluate(*t_, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 1);
  }
}

TEST_F(VariableTest, SERVER_PORT) {
  Common::EvaluateResults result;

  // Test getting server port
  {
    Variable::ServerPort server_port("", false, false, "");
    result.clear();
    server_port.evaluate(*t_, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 80);
  }

  // Test counter mode
  {
    Variable::ServerPort server_port_count("", false, true, "");
    result.clear();
    server_port_count.evaluate(*t_, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 1);
  }
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
  Common::EvaluateResults result;

  // Test TIME_DAY - returns day of month (1-31)
  Variable::TimeDay time_day("", false, false, "");
  result.clear();
  time_day.evaluate(*t_, result);
  EXPECT_EQ(result.size(), 1);
  auto day = std::get<int64_t>(result.front().variant_);
  EXPECT_GE(day, 1);
  EXPECT_LE(day, 31);
}

TEST_F(VariableTest, TIME_EPOCH) {
  Common::EvaluateResults result;

  // Test TIME_EPOCH - returns Unix timestamp
  Variable::TimeEpoch time_epoch("", false, false, "");
  result.clear();
  time_epoch.evaluate(*t_, result);
  EXPECT_EQ(result.size(), 1);
  auto epoch = std::get<int64_t>(result.front().variant_);
  // Should be a positive value (seconds since 1970)
  EXPECT_GT(epoch, 0);
}

TEST_F(VariableTest, TIME_HOUR) {
  Common::EvaluateResults result;

  // Test TIME_HOUR - returns hour (0-23)
  Variable::TimeHour time_hour("", false, false, "");
  result.clear();
  time_hour.evaluate(*t_, result);
  EXPECT_EQ(result.size(), 1);
  auto hour = std::get<int64_t>(result.front().variant_);
  EXPECT_GE(hour, 0);
  EXPECT_LE(hour, 23);
}

TEST_F(VariableTest, TIME_MIN) {
  Common::EvaluateResults result;

  // Test TIME_MIN - returns minute (0-59)
  Variable::TimeMin time_min("", false, false, "");
  result.clear();
  time_min.evaluate(*t_, result);
  EXPECT_EQ(result.size(), 1);
  auto min = std::get<int64_t>(result.front().variant_);
  EXPECT_GE(min, 0);
  EXPECT_LE(min, 59);
}

TEST_F(VariableTest, TIME_MON) {
  Common::EvaluateResults result;

  // Test TIME_MON - returns month (1-12)
  Variable::TimeMon time_mon("", false, false, "");
  result.clear();
  time_mon.evaluate(*t_, result);
  EXPECT_EQ(result.size(), 1);
  auto mon = std::get<int64_t>(result.front().variant_);
  EXPECT_GE(mon, 1);
  EXPECT_LE(mon, 12);
}

TEST_F(VariableTest, TIME_SEC) {
  Common::EvaluateResults result;

  // Test TIME_SEC - returns second (0-59)
  Variable::TimeSec time_sec("", false, false, "");
  result.clear();
  time_sec.evaluate(*t_, result);
  EXPECT_EQ(result.size(), 1);
  auto sec = std::get<int64_t>(result.front().variant_);
  EXPECT_GE(sec, 0);
  EXPECT_LE(sec, 59);
}

TEST_F(VariableTest, TIME_WDAY) {
  Common::EvaluateResults result;

  // Test TIME_WDAY - returns day of week (0-6, Sunday=0)
  Variable::TimeWDay time_wday("", false, false, "");
  result.clear();
  time_wday.evaluate(*t_, result);
  EXPECT_EQ(result.size(), 1);
  auto wday = std::get<int64_t>(result.front().variant_);
  EXPECT_GE(wday, 0);
  EXPECT_LE(wday, 6);
}

TEST_F(VariableTest, TIME_YEAR) {
  Common::EvaluateResults result;

  Variable::TimeYear time_year("", false, false, "");
  result.clear();
  time_year.evaluate(*t_, result);
  EXPECT_EQ(result.size(), 1);
  auto year = std::get<int64_t>(result.front().variant_);
  EXPECT_GT(year, 1900);
}

TEST_F(VariableTest, TIME) {
  Common::EvaluateResults result;

  // Test TIME - returns time in HHMMSS format
  Variable::Time time("", false, false, "");
  result.clear();
  time.evaluate(*t_, result);
  EXPECT_EQ(result.size(), 1);
  auto time_value = std::get<int64_t>(result.front().variant_);
  // Should be in format HHMMSS
  EXPECT_GE(time_value, 0);
  EXPECT_LE(time_value, 235959);
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
  Common::EvaluateResults result;

  // Test unique ID - should return a non-empty unique identifier
  {
    Variable::UniqueId unique_id("", false, false, "");
    result.clear();
    unique_id.evaluate(*t_, result);
    EXPECT_EQ(result.size(), 1);
    std::string_view id = std::get<std::string_view>(result.front().variant_);
    EXPECT_FALSE(id.empty());
    // Unique ID format: timestamp.random (e.g., "1234567890.123456")
    EXPECT_GT(id.size(), 0);
  }

  // Test counter mode
  {
    Variable::UniqueId unique_id_count("", false, true, "");
    result.clear();
    unique_id_count.evaluate(*t_, result);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result.front().variant_), 1);
  }

  // Test that different transactions have different unique IDs
  {
    Variable::UniqueId unique_id1("", false, false, "");
    result.clear();
    unique_id1.evaluate(*t_, result);
    std::string_view id1 = std::get<std::string_view>(result.front().variant_);

    // Create a new transaction
    auto t2 = engine_.makeTransaction();
    t2->processConnection(downstream_ip_, downstream_port_, upstream_ip_, upstream_port_);
    t2->processUri(uri_, method_, version_);
    t2->processRequestHeaders(request_header_find_, request_header_traversal_,
                              request_headers_.size(), nullptr);

    Variable::UniqueId unique_id2("", false, false, "");
    result.clear();
    unique_id2.evaluate(*t2, result);
    std::string_view id2 = std::get<std::string_view>(result.front().variant_);

    // Two transactions should have different unique IDs
    EXPECT_NE(id1, id2);
  }
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
  Common::EvaluateResults result;

  {
    Variable::PTree var("config.max_connection", false, false, "");
    Variable::PTree var1("config.max_connection", false, true, "");
    result.clear();
    var.evaluate(*t_, result);
    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result[0].variant_), 100);

    result.clear();
    var1.evaluate(*t_, result);
    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result[0].variant_), 1);
  }

  {
    Variable::PTree var("config.server_list[].host", false, false, "");
    Variable::PTree var1("config.server_list[].host", false, true, "");
    result.clear();
    var.evaluate(*t_, result);
    ASSERT_EQ(result.size(), 2);
    EXPECT_EQ(std::get<std::string_view>(result[0].variant_), "192.168.1.1");
    EXPECT_EQ(std::get<std::string_view>(result[1].variant_), "192.168.1.2");

    result.clear();
    var1.evaluate(*t_, result);
    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result[0].variant_), 2);
  }

  {
    Variable::PTree var("config.server_list[].port", false, false, "");
    Variable::PTree var1("config.server_list[].port", false, true, "");
    result.clear();
    var.evaluate(*t_, result);
    ASSERT_EQ(result.size(), 2);
    EXPECT_EQ(std::get<int64_t>(result[0].variant_), 8080);
    EXPECT_EQ(std::get<int64_t>(result[1].variant_), 8081);

    result.clear();
    var1.evaluate(*t_, result);
    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result[0].variant_), 2);
  }

  {
    Variable::PTree var("config.server_list[].domain{}", false, false, "");
    Variable::PTree var1("config.server_list[].domain{}", false, true, "");
    result.clear();
    var.evaluate(*t_, result);
    ASSERT_EQ(result.size(), 4);
    EXPECT_EQ(std::get<std::string_view>(result[0].variant_), "server1.example.com");
    EXPECT_EQ(result[0].variable_sub_name_, "name");
    EXPECT_EQ(std::get<std::string_view>(result[1].variant_), "2025-12-31");
    EXPECT_EQ(result[1].variable_sub_name_, "expire_time");
    EXPECT_EQ(std::get<std::string_view>(result[2].variant_), "server2.example.com");
    EXPECT_EQ(result[2].variable_sub_name_, "name");
    EXPECT_EQ(std::get<std::string_view>(result[3].variant_), "2025-12-31");
    EXPECT_EQ(result[3].variable_sub_name_, "expire_time");

    result.clear();
    var1.evaluate(*t_, result);
    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result[0].variant_), 4);
  }

  {
    Variable::PTree var("config.server_list[].domain.name", false, false, "");
    Variable::PTree var1("config.server_list[].domain.name", false, true, "");
    result.clear();
    var.evaluate(*t_, result);
    ASSERT_EQ(result.size(), 2);
    EXPECT_EQ(std::get<std::string_view>(result[0].variant_), "server1.example.com");
    EXPECT_EQ(result[0].variable_sub_name_, "name");
    EXPECT_EQ(std::get<std::string_view>(result[1].variant_), "server2.example.com");
    EXPECT_EQ(result[1].variable_sub_name_, "name");

    result.clear();
    var1.evaluate(*t_, result);
    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result[0].variant_), 2);
  }

  {
    Variable::PTree var("config.server_list[].tags[]", false, false, "");
    Variable::PTree var1("config.server_list[].tags[]", false, true, "");
    result.clear();
    var.evaluate(*t_, result);
    ASSERT_EQ(result.size(), 4);
    EXPECT_EQ(std::get<std::string_view>(result[0].variant_), "production");
    EXPECT_EQ(std::get<std::string_view>(result[1].variant_), "v1.0");
    EXPECT_EQ(std::get<std::string_view>(result[2].variant_), "staging");
    EXPECT_EQ(std::get<std::string_view>(result[3].variant_), "v1.1");

    result.clear();
    var1.evaluate(*t_, result);
    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result[0].variant_), 4);
  }

  {
    Variable::PTree var("config.boolean_test1", false, false, "");
    result.clear();
    var.evaluate(*t_, result);
    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<std::int64_t>(result[0].variant_), 1);
  }

  {
    Variable::PTree var("config.boolean_test2", false, false, "");
    result.clear();
    var.evaluate(*t_, result);
    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<std::int64_t>(result[0].variant_), 0);
  }

  {
    Variable::PTree var("config.null_test", false, false, "");
    result.clear();
    var.evaluate(*t_, result);
    ASSERT_EQ(result.size(), 1);
    EXPECT_TRUE(IS_EMPTY_VARIANT(result[0].variant_));
  }

  {
    Variable::PTree var("config.float_test", false, false, "");
    result.clear();
    var.evaluate(*t_, result);
    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result[0].variant_), 314);
  }

  {
    Variable::PTree var("config.minus_test", false, false, "");
    result.clear();
    var.evaluate(*t_, result);
    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result[0].variant_), -100);
  }

  {
    Variable::PTree var("config.minus_float_test", false, false, "");
    result.clear();
    var.evaluate(*t_, result);
    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result[0].variant_), -314);
  }
}

TEST_F(VariableTest, MATCHED_OPTREE) {
  Common::EvaluateResults result;

  // config.server_list[].port../host
  {
    auto& matched_optree =
        t_->propertyTree()->get_child("config.server_list").front().second.get_child("port");
    EXPECT_EQ(std::get<int64_t>(matched_optree.data()), 8080);
    t_->pushMatchedOPTree(-1, static_cast<const Common::PropertyTree*>(&matched_optree));
    Variable::MatchedOPTree var("../host", false, false, "");
    Variable::MatchedOPTree var1("../host", false, true, "");
    result.clear();
    var.evaluate(*t_, result);
    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<std::string_view>(result[0].variant_), "192.168.1.1");

    result.clear();
    var1.evaluate(*t_, result);
    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result[0].variant_), 1);
  }

  // config.server_list[].tags[]
  {
    auto& matched_optree = t_->propertyTree()->get_child("config");
    t_->pushMatchedOPTree(-1, static_cast<const Common::PropertyTree*>(&matched_optree));
    Variable::MatchedOPTree var("server_list[].tags[]", false, false, "");
    Variable::MatchedOPTree var1("server_list[].tags[]", false, true, "");
    result.clear();
    var.evaluate(*t_, result);
    ASSERT_EQ(result.size(), 4);
    EXPECT_EQ(std::get<std::string_view>(result[0].variant_), "production");
    EXPECT_EQ(std::get<std::string_view>(result[1].variant_), "v1.0");
    EXPECT_EQ(std::get<std::string_view>(result[2].variant_), "staging");
    EXPECT_EQ(std::get<std::string_view>(result[3].variant_), "v1.1");

    result.clear();
    var1.evaluate(*t_, result);
    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result[0].variant_), 4);
  }

  // config.server_list[].tags[]../../domain.name
  {
    auto& matched_optree = t_->propertyTree()
                               ->get_child("config.server_list")
                               .back()
                               .second.get_child("tags")
                               .front()
                               .second;
    EXPECT_EQ(std::get<std::string_view>(matched_optree.data()), "staging");
    t_->pushMatchedOPTree(-1, static_cast<const Common::PropertyTree*>(&matched_optree));
    Variable::MatchedOPTree var("../../domain.name", false, false, "");
    Variable::MatchedOPTree var1("../../domain.name", false, true, "");
    result.clear();
    var.evaluate(*t_, result);
    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<std::string_view>(result[0].variant_), "server2.example.com");

    result.clear();
    var1.evaluate(*t_, result);
    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result[0].variant_), 1);
  }
}

TEST_F(VariableTest, MATCHED_VPTREE) {
  Common::EvaluateResults result;

  // config.server_list[].port../host
  {
    auto& matched_vptree =
        t_->propertyTree()->get_child("config.server_list").front().second.get_child("port");
    EXPECT_EQ(std::get<int64_t>(matched_vptree.data()), 8080);
    t_->pushMatchedVPTree(-1, static_cast<const Common::PropertyTree*>(&matched_vptree));
    Variable::MatchedVPTree var("../host", false, false, "");
    Variable::MatchedVPTree var1("../host", false, true, "");
    result.clear();
    var.evaluate(*t_, result);
    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<std::string_view>(result[0].variant_), "192.168.1.1");

    result.clear();
    var1.evaluate(*t_, result);
    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result[0].variant_), 1);
  }

  // config.server_list[].tags[]
  {
    auto& matched_vptree = t_->propertyTree()->get_child("config");
    t_->pushMatchedVPTree(-1, static_cast<const Common::PropertyTree*>(&matched_vptree));
    Variable::MatchedVPTree var("server_list[].tags[]", false, false, "");
    Variable::MatchedVPTree var1("server_list[].tags[]", false, true, "");
    result.clear();
    var.evaluate(*t_, result);
    ASSERT_EQ(result.size(), 4);
    EXPECT_EQ(std::get<std::string_view>(result[0].variant_), "production");
    EXPECT_EQ(std::get<std::string_view>(result[1].variant_), "v1.0");
    EXPECT_EQ(std::get<std::string_view>(result[2].variant_), "staging");
    EXPECT_EQ(std::get<std::string_view>(result[3].variant_), "v1.1");

    result.clear();
    var1.evaluate(*t_, result);
    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result[0].variant_), 4);
  }

  // config.server_list[].tags[]../../domain.name
  {
    auto& matched_vptree = t_->propertyTree()
                               ->get_child("config.server_list")
                               .back()
                               .second.get_child("tags")
                               .front()
                               .second;
    EXPECT_EQ(std::get<std::string_view>(matched_vptree.data()), "staging");
    t_->pushMatchedVPTree(-1, static_cast<const Common::PropertyTree*>(&matched_vptree));
    Variable::MatchedVPTree var("../../domain.name", false, false, "");
    Variable::MatchedVPTree var1("../../domain.name", false, true, "");
    result.clear();
    var.evaluate(*t_, result);
    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<std::string_view>(result[0].variant_), "server2.example.com");

    result.clear();
    var1.evaluate(*t_, result);
    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result[0].variant_), 1);
  }
}

TEST_F(VariableTest, CURRENT_OPTREE) {
  Common::EvaluateResults result;

  // config.server_list[].port../host
  {
    auto& matched_optree =
        t_->propertyTree()->get_child("config.server_list").front().second.get_child("port");
    EXPECT_EQ(std::get<int64_t>(matched_optree.data()), 8080);
    t_->pushMatchedOPTree(-1, static_cast<const Common::PropertyTree*>(&matched_optree));
    Variable::CurrentOPTree var("../host", false, false, "");
    Variable::CurrentOPTree var1("../host", false, true, "");
    result.clear();
    var.evaluate(*t_, result);
    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<std::string_view>(result[0].variant_), "192.168.1.1");

    result.clear();
    var1.evaluate(*t_, result);
    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result[0].variant_), 1);
  }

  // config.server_list[].tags[]
  {
    auto& matched_optree = t_->propertyTree()->get_child("config");
    t_->pushMatchedOPTree(-1, static_cast<const Common::PropertyTree*>(&matched_optree));
    Variable::CurrentOPTree var("server_list[].tags[]", false, false, "");
    Variable::CurrentOPTree var1("server_list[].tags[]", false, true, "");
    result.clear();
    var.evaluate(*t_, result);
    ASSERT_EQ(result.size(), 4);
    EXPECT_EQ(std::get<std::string_view>(result[0].variant_), "production");
    EXPECT_EQ(std::get<std::string_view>(result[1].variant_), "v1.0");
    EXPECT_EQ(std::get<std::string_view>(result[2].variant_), "staging");
    EXPECT_EQ(std::get<std::string_view>(result[3].variant_), "v1.1");

    result.clear();
    var1.evaluate(*t_, result);
    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result[0].variant_), 4);
  }

  // config.server_list[].tags[]../../domain.name
  {
    auto& matched_optree = t_->propertyTree()
                               ->get_child("config.server_list")
                               .back()
                               .second.get_child("tags")
                               .front()
                               .second;
    EXPECT_EQ(std::get<std::string_view>(matched_optree.data()), "staging");
    t_->pushMatchedOPTree(-1, static_cast<const Common::PropertyTree*>(&matched_optree));
    Variable::CurrentOPTree var("../../domain.name", false, false, "");
    Variable::CurrentOPTree var1("../../domain.name", false, true, "");
    result.clear();
    var.evaluate(*t_, result);
    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<std::string_view>(result[0].variant_), "server2.example.com");

    result.clear();
    var1.evaluate(*t_, result);
    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result[0].variant_), 1);
  }
}

TEST_F(VariableTest, CURRENT_VPTREE) {
  Common::EvaluateResults result;

  // config.server_list[].port../host
  {
    auto& matched_vptree =
        t_->propertyTree()->get_child("config.server_list").front().second.get_child("port");
    EXPECT_EQ(std::get<int64_t>(matched_vptree.data()), 8080);
    t_->pushMatchedVPTree(-1, static_cast<const Common::PropertyTree*>(&matched_vptree));
    Variable::CurrentVPTree var("../host", false, false, "");
    Variable::CurrentVPTree var1("../host", false, true, "");
    result.clear();
    var.evaluate(*t_, result);
    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<std::string_view>(result[0].variant_), "192.168.1.1");

    result.clear();
    var1.evaluate(*t_, result);
    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result[0].variant_), 1);
  }

  // config.server_list[].tags[]
  {
    auto& matched_vptree = t_->propertyTree()->get_child("config");
    t_->pushMatchedVPTree(-1, static_cast<const Common::PropertyTree*>(&matched_vptree));
    Variable::CurrentVPTree var("server_list[].tags[]", false, false, "");
    Variable::CurrentVPTree var1("server_list[].tags[]", false, true, "");
    result.clear();
    var.evaluate(*t_, result);
    ASSERT_EQ(result.size(), 4);
    EXPECT_EQ(std::get<std::string_view>(result[0].variant_), "production");
    EXPECT_EQ(std::get<std::string_view>(result[1].variant_), "v1.0");
    EXPECT_EQ(std::get<std::string_view>(result[2].variant_), "staging");
    EXPECT_EQ(std::get<std::string_view>(result[3].variant_), "v1.1");

    result.clear();
    var1.evaluate(*t_, result);
    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result[0].variant_), 4);
  }

  // config.server_list[].tags[]../../domain.name
  {
    auto& matched_vptree = t_->propertyTree()
                               ->get_child("config.server_list")
                               .back()
                               .second.get_child("tags")
                               .front()
                               .second;
    EXPECT_EQ(std::get<std::string_view>(matched_vptree.data()), "staging");
    t_->pushMatchedVPTree(-1, static_cast<const Common::PropertyTree*>(&matched_vptree));
    Variable::CurrentVPTree var("../../domain.name", false, false, "");
    Variable::CurrentVPTree var1("../../domain.name", false, true, "");
    result.clear();
    var.evaluate(*t_, result);
    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<std::string_view>(result[0].variant_), "server2.example.com");

    result.clear();
    var1.evaluate(*t_, result);
    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(std::get<int64_t>(result[0].variant_), 1);
  }
}

TEST_F(VariableTest, SubnameWithMacro) {
  Common::EvaluateResults result;

  std::unique_ptr<Variable::PTree> var =
      std::make_unique<Variable::PTree>("config.server_list[].host", false, false, "");
  std::unique_ptr<Macro::VariableMacro> macro =
      std::make_unique<Macro::VariableMacro>("%{PTREE.config.server_list[].host}", std::move(var));

  t_->setVariable("", "192.168.1.1", "value1");
  t_->setVariable("", "192.168.1.2", "value2");

  Variable::Tx tx_var("", std::move(macro), std::nullopt, false, false, "");
  tx_var.evaluate(*t_, result);
  EXPECT_EQ(result.size(), 2);
  EXPECT_EQ(std::get<std::string_view>(result[0].variant_), "value1");
  EXPECT_EQ(std::get<std::string_view>(result[1].variant_), "value2");
}

} // namespace Integration
} // namespace Wge