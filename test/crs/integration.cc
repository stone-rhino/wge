#include <unordered_map>

#include <gtest/gtest.h>

#include "engine.h"

namespace SrSecurity {
class IntegrationTest : public testing::Test {
public:
  IntegrationTest() : engine_(spdlog::level::trace) {}

public:
  void SetUp() override {
    std::expected<bool, std::string> result;
    std::vector<std::string> rule_files = {
        "test/test_data/waf-conf/base/engin-setup.conf",
        "test/test_data/waf-conf/base/crs-setup.conf",
        "test/test_data/waf-conf/coreruleset/rules/REQUEST-901-INITIALIZATION.conf",
        "test/test_data/waf-conf/coreruleset/rules/REQUEST-905-COMMON-EXCEPTIONS.conf",
        "test/test_data/waf-conf/coreruleset/rules/REQUEST-911-METHOD-ENFORCEMENT.conf",
        "test/test_data/waf-conf/coreruleset/rules/REQUEST-913-SCANNER-DETECTION.conf",
        "test/test_data/waf-conf/coreruleset/rules/REQUEST-920-PROTOCOL-ENFORCEMENT.conf",
        "test/test_data/waf-conf/coreruleset/rules/REQUEST-921-PROTOCOL-ATTACK.conf",
        "test/test_data/waf-conf/coreruleset/rules/REQUEST-922-MULTIPART-ATTACK.conf",
        "test/test_data/waf-conf/coreruleset/rules/REQUEST-930-APPLICATION-ATTACK-LFI.conf",
        "test/test_data/waf-conf/coreruleset/rules/REQUEST-931-APPLICATION-ATTACK-RFI.conf",
        "test/test_data/waf-conf/coreruleset/rules/REQUEST-932-APPLICATION-ATTACK-RCE.conf",
        "test/test_data/waf-conf/coreruleset/rules/REQUEST-933-APPLICATION-ATTACK-PHP.conf",
        "test/test_data/waf-conf/coreruleset/rules/REQUEST-934-APPLICATION-ATTACK-GENERIC.conf",
        "test/test_data/waf-conf/coreruleset/rules/REQUEST-941-APPLICATION-ATTACK-XSS.conf",
        "test/test_data/waf-conf/coreruleset/rules/REQUEST-942-APPLICATION-ATTACK-SQLI.conf",
        "test/test_data/waf-conf/coreruleset/rules/"
        "REQUEST-943-APPLICATION-ATTACK-SESSION-FIXATION.conf",
        "test/test_data/waf-conf/coreruleset/rules/REQUEST-944-APPLICATION-ATTACK-JAVA.conf",
        "test/test_data/waf-conf/coreruleset/rules/REQUEST-949-BLOCKING-EVALUATION.conf",
        "test/test_data/waf-conf/coreruleset/rules/RESPONSE-950-DATA-LEAKAGES.conf",
        "test/test_data/waf-conf/coreruleset/rules/RESPONSE-951-DATA-LEAKAGES-SQL.conf",
        "test/test_data/waf-conf/coreruleset/rules/RESPONSE-952-DATA-LEAKAGES-JAVA.conf",
        "test/test_data/waf-conf/coreruleset/rules/RESPONSE-953-DATA-LEAKAGES-PHP.conf",
        "test/test_data/waf-conf/coreruleset/rules/RESPONSE-954-DATA-LEAKAGES-IIS.conf",
        "test/test_data/waf-conf/coreruleset/rules/RESPONSE-955-DATA-LEAKAGES-APACHE.conf",
        "test/test_data/waf-conf/coreruleset/rules/RESPONSE-959-BLOCKING-EVALUATION.conf",
        "test/test_data/waf-conf/coreruleset/rules/RESPONSE-980-CORRELATION.conf",
    };
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

  std::string uri_{"/"};
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

TEST(IntegrationPrev, ruleEvaluateLogic) {
  // Test that all variables will be evaluated and the action will be executed every time when the
  // each variable is matched.
  // And any variable is matched, the rule will be matched, and msg and logdata macro will be
  // evaluated.
  {
    const std::string directive = R"(
      SecRuleEngine On
      SecAction "phase:1,setvar:tx.foo1=bar,setvar:tx.foo2=bar123,setvar:tx.foo3=bar,setvar:tx.foo4=BAR"
      SecRule TX:foo1|TX:foo2|TX:foo3|TX:foo4 "@streq bar" \
      "id:1, \
      phase:1, \
      pass, \
      t:none, \
      t:lowercase, \
      msg:'tx.test=%{tx.test}', \
      logdata:'%{MATCHED_VAR_NAME}=%{MATCHED_VAR} %{MATCHED_VARS_NAMES}=%{MATCHED_VARS}', \
      setvar:tx.test=+1")";

    Engine engine(spdlog::level::trace);
    auto result = engine.load(directive);
    engine.init();
    auto t = engine.makeTransaction();
    ASSERT_TRUE(result.has_value());

    bool matched = false;
    t->processRequestHeaders(nullptr, nullptr, 0, [&](const Rule& rule) { matched = true; });
    EXPECT_EQ(std::get<int>(t->getVariable("test")), 3);
    EXPECT_TRUE(matched);
    EXPECT_EQ(t->getMsgMacroExpanded(), "tx.test=3");
    EXPECT_EQ(t->getLogDataMacroExpanded(), "TX:foo4=bar TX:foo1=bar");
  }

  // Test that chained rule is matched, and starter rule is matched.
  {
    const std::string directive = R"(
      SecRuleEngine On
      SecAction "phase:1,setvar:tx.foo1=bar,setvar:tx.foo2=bar123,setvar:tx.foo3=bar,setvar:tx.foo4=BAR"
      SecRule TX:foo1|TX:foo2|TX:foo3|TX:foo4 "@streq bar" \
      "id:1, \
      phase:1, \
      pass, \
      t:none, \
      t:lowercase, \
      msg:'tx.test=%{tx.test}', \
      logdata:'%{MATCHED_VAR_NAME}=%{MATCHED_VAR} %{MATCHED_VARS_NAMES}=%{MATCHED_VARS}', \
      chain, \
      setvar:tx.test=+1"
        SecRule TX:foo1 "@streq bar" "setvar:tx.chain=true")";

    Engine engine(spdlog::level::trace);
    auto result = engine.load(directive);
    engine.init();
    auto t = engine.makeTransaction();
    ASSERT_TRUE(result.has_value());

    bool matched = false;
    t->processRequestHeaders(nullptr, nullptr, 0, [&](const Rule& rule) { matched = true; });
    EXPECT_EQ(std::get<int>(t->getVariable("test")), 3);
    EXPECT_TRUE(matched);
    EXPECT_EQ(t->getMsgMacroExpanded(), "tx.test=3");
    EXPECT_EQ(t->getLogDataMacroExpanded(), "TX:foo1=bar TX:foo1=bar");
    EXPECT_EQ(std::get<std::string_view>(t->getVariable("chain")), "true");
  }

  // Test that chained rule is not matched, and starter rule is not matched, and the msg and logdata
  // macro will not be evaluated. But the action will be executed.
  {
    const std::string directive = R"(
      SecRuleEngine On
      SecAction "phase:1,setvar:tx.foo1=bar,setvar:tx.foo2=bar123,setvar:tx.foo3=bar,setvar:tx.foo4=BAR"
      SecRule TX:foo1|TX:foo2|TX:foo3|TX:foo4 "@streq bar" \
      "id:1, \
      phase:1, \
      pass, \
      t:none, \
      t:lowercase, \
      msg:'tx.test=%{tx.test}', \
      logdata:'%{MATCHED_VAR_NAME}=%{MATCHED_VAR}  %{MATCHED_VARS_NAMES}=%{MATCHED_VARS}', \
      chain, \
      setvar:tx.test=+1"
        SecRule TX:foo1 "@streq bar12" "setvar:tx.chain=true")";

    Engine engine(spdlog::level::trace);
    auto result = engine.load(directive);
    engine.init();
    auto t = engine.makeTransaction();
    ASSERT_TRUE(result.has_value());

    bool matched = false;
    t->processRequestHeaders(nullptr, nullptr, 0, [&](const Rule& rule) { matched = true; });
    EXPECT_EQ(std::get<int>(t->getVariable("test")), 3);
    EXPECT_FALSE(matched);
    EXPECT_TRUE(t->getMsgMacroExpanded().empty());
    EXPECT_TRUE(t->getLogDataMacroExpanded().empty());
  }
}

TEST(IntegrationPrev, ruleExceptVariable) {
  // Test that the except variable is won't be evaluated.
  {
    const std::string directive = R"(
      SecRuleEngine On
      SecAction "phase:1,setvar:tx.foo1=bar,setvar:tx.foo2=bar123,setvar:tx.foo3=bar,setvar:tx.foo4=BAR"
      SecRule TX:foo1|TX:foo2|TX:foo3|TX:foo4|!TX:foo1|TX  "@streq bar" \
      "id:1, \
      phase:1, \
      pass, \
      t:none, \
      t:lowercase, \
      msg:'tx.test=%{tx.test}', \
      logdata:'%{MATCHED_VAR_NAME}=%{MATCHED_VAR} %{MATCHED_VARS_NAMES}=%{MATCHED_VARS}', \
      setvar:tx.test=+1")";

    Engine engine(spdlog::level::trace);
    auto result = engine.load(directive);
    engine.init();
    auto t = engine.makeTransaction();
    ASSERT_TRUE(result.has_value());

    bool matched = false;
    t->processRequestHeaders(nullptr, nullptr, 0, [&](const Rule& rule) { matched = true; });
    EXPECT_EQ(std::get<int>(t->getVariable("test")), 4);
    EXPECT_TRUE(matched);
    EXPECT_EQ(t->getMsgMacroExpanded(), "tx.test=4");
    EXPECT_EQ(t->getLogDataMacroExpanded(), "TX:test=2 TX:foo2=bar123");
  }

  // Test that the except collection is won't be evaluated.
  {
    const std::string directive = R"(
      SecRuleEngine On
      SecAction "phase:1,setvar:tx.foo1=bar,setvar:tx.foo2=bar123,setvar:tx.foo3=bar,setvar:tx.foo4=BAR"
      SecRule TX:foo1|TX:foo2|TX:foo3|TX:foo4|!TX "@streq bar" \
      "id:1, \
      phase:1, \
      pass, \
      t:none, \
      t:lowercase, \
      msg:'tx.test=%{tx.test}', \
      logdata:'%{MATCHED_VAR_NAME}=%{MATCHED_VAR} %{MATCHED_VARS_NAMES}=%{MATCHED_VARS}', \
      setvar:tx.test=+1")";

    Engine engine(spdlog::level::trace);
    auto result = engine.load(directive);
    engine.init();
    auto t = engine.makeTransaction();
    ASSERT_TRUE(result.has_value());

    bool matched = false;
    t->processRequestHeaders(nullptr, nullptr, 0, [&](const Rule& rule) { matched = true; });
    EXPECT_FALSE(t->hasVariable("test"));
    EXPECT_FALSE(matched);
  }
}

TEST_F(IntegrationTest, crs) {
  auto t = engine_.makeTransaction();
  t->processConnection(downstream_ip_, downstream_port_, upstream_ip_, upstream_port_);
  t->processUri(uri_, method_, version_);
  t->processRequestHeaders(request_header_find_, request_header_traversal_, request_headers_.size(),
                           nullptr);
}
} // namespace SrSecurity