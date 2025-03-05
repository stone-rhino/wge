#include <unordered_map>

#include <gtest/gtest.h>

#include "engine.h"

namespace SrSecurity {
class IntegrationTest : public testing::Test {
public:
  IntegrationTest() : engine_(spdlog::level::trace) {}

public:
  void SetUp() override {
    auto result = engine_.loadFromFile(
        "test/test_data/waf-conf/coreruleset/rules/REQUEST-901-INITIALIZATION.conf");
    EXPECT_TRUE(result.has_value());
    if (!result.has_value()) {
      std::cout << result.error() << std::endl;
    }

    engine_.init();

    conn_extractor_ = [&](std::string_view& downstream_ip, short& downstream_port,
                          std::string_view& upstream_ip, short& upstream_port) {
      downstream_ip = downstream_ip_;
      downstream_port = downstream_port_;
      upstream_ip = upstream_ip_;
      upstream_port = upstream_port_;
    };

    uri_extractor_ = [&](std::string_view& method, std::string_view& path,
                         std::string_view& protocol, std::string_view& version) {
      method = method_;
      path = path_;
      protocol = protocol_;
      version = version_;
    };

    request_header_extractor_ = [&](const std::string& key) {
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

    request_body_extractor_ = [&]() -> const std::vector<std::string_view>& {
      return request_body_;
    };
  }

protected:
  Engine engine_;
  ConnectionExtractor conn_extractor_;
  UriExtractor uri_extractor_;
  HeaderExtractor request_header_extractor_;
  BodyExtractor request_body_extractor_;
  HeaderExtractor response_header_extractor_;
  BodyExtractor response_body_extractor_;

private:
  std::string downstream_ip_{"192.168.1.100"};
  short downstream_port_{20000};
  std::string upstream_ip_{"192.168.1.200"};
  short upstream_port_{80};

  std::string method_{"Get"};
  std::string path_{"/"};
  std::string protocol_{"HTTP"};
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

TEST_F(IntegrationTest, crs) {
  auto t = engine_.makeTransaction();
  t->processConnection(conn_extractor_);
  t->processUri(uri_extractor_);
  t->processRequestHeaders(request_header_extractor_, nullptr);
}
} // namespace SrSecurity