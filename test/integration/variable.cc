#include <gtest/gtest.h>

#include "engine.h"
#include "variable/variables_include.h"

namespace SrSecurity {
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
  BodyExtractor request_body_extractor_;
  HeaderFind response_header_find_;
  HeaderTraversal response_header_traversal_;
  BodyExtractor response_body_extractor_;

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

  std::vector<std::string_view> request_body_;
};

TEST_F(VariableTest, ARGS_GET_NAMES) {
  Common::EvaluateResults result;

  Variable::ArgsGetNames all("", false, false);
  result.clear();
  all.evaluate(*t_, result);
  EXPECT_EQ(result.size(), 4);

  Variable::ArgsGetNames all_count("", false, true);
  result.clear();
  all_count.evaluate(*t_, result);
  EXPECT_EQ(result.size(), 1);
  EXPECT_EQ(std::get<int>(result.front().variant_), 4);

  Variable::ArgsGetNames sub("p1", false, false);
  result.clear();
  sub.evaluate(*t_, result);
  EXPECT_EQ(result.size(), 1);
  EXPECT_EQ(std::get<std::string_view>(result.front().variant_), "p1");

  Variable::ArgsGetNames sub_count("p1", false, true);
  result.clear();
  sub_count.evaluate(*t_, result);
  EXPECT_EQ(result.size(), 1);
  EXPECT_EQ(std::get<int>(result.front().variant_), 1);

  {
    Variable::ArgsGetNames sub_regex("/^p/", false, false);
    result.clear();
    sub_regex.evaluate(*t_, result);
    EXPECT_EQ(result.size(), 4);
  }

  {
    Variable::ArgsGetNames sub_regex("/^p\\d/", false, false);
    result.clear();
    sub_regex.evaluate(*t_, result);
    EXPECT_EQ(result.size(), 4);
  }

  {
    Variable::ArgsGetNames sub_regex("/^pa/", false, false);
    result.clear();
    sub_regex.evaluate(*t_, result);
    EXPECT_EQ(result.size(), 0);
  }
}

TEST_F(VariableTest, ARGS_GET) {
  Common::EvaluateResults result;

  Variable::ArgsGet all("", false, false);
  result.clear();
  all.evaluate(*t_, result);
  EXPECT_EQ(result.size(), 4);

  Variable::ArgsGet all_count("", false, true);
  result.clear();
  all_count.evaluate(*t_, result);
  EXPECT_EQ(result.size(), 1);
  EXPECT_EQ(std::get<int>(result.front().variant_), 4);

  Variable::ArgsGet sub("p1", false, false);
  result.clear();
  sub.evaluate(*t_, result);
  EXPECT_EQ(result.size(), 1);
  EXPECT_EQ(std::get<std::string_view>(result.front().variant_), "v1");

  Variable::ArgsGet sub_count("p1", false, true);
  result.clear();
  sub_count.evaluate(*t_, result);
  EXPECT_EQ(result.size(), 1);
  EXPECT_EQ(std::get<int>(result.front().variant_), 1);

  {
    Variable::ArgsGet sub_regex("/^p/", false, false);
    result.clear();
    sub_regex.evaluate(*t_, result);
    EXPECT_EQ(result.size(), 4);
  }

  {
    Variable::ArgsGet sub_regex("/^p\\d/", false, false);
    result.clear();
    sub_regex.evaluate(*t_, result);
    EXPECT_EQ(result.size(), 4);
  }

  {
    Variable::ArgsGet sub_regex("/^pa/", false, false);
    result.clear();
    sub_regex.evaluate(*t_, result);
    EXPECT_EQ(result.size(), 0);
  }
}
} // namespace Integration
} // namespace SrSecurity