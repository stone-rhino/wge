#include "antlr4/parser.h"

#include <functional>
#include <string>
#include <vector>

#include <gtest/gtest.h>

namespace SrSecurity {
class ParserTest : public testing::Test {
public:
  const std::vector<std::unique_ptr<Variable::VariableBase>>& ruleVariablePool(Rule& rule) const {
    return rule.variables_pool_;
  }

  const std::unordered_map<std::string, Variable::VariableBase&>&
  ruleVariableMap(Rule& rule) const {
    return rule.variables_map_;
  }

public:
  static const std::vector<std::string> test_files_;
};

// clang-format off
const std::vector<std::string> ParserTest::test_files_({
  "waf-conf/base/before.conf", 
  "waf-conf/coreruleset/rules/REQUEST-913-SCANNER-DETECTION.conf",
  "waf-conf/coreruleset/rules/REQUEST-920-PROTOCOL-ENFORCEMENT.conf",
  "waf-conf/coreruleset/rules/REQUEST-921-PROTOCOL-ATTACK.conf",
  "waf-conf/coreruleset/rules/REQUEST-922-MULTIPART-ATTACK.conf",
  "waf-conf/coreruleset/rules/REQUEST-930-APPLICATION-ATTACK-LFI.conf",
  "waf-conf/coreruleset/rules/REQUEST-931-APPLICATION-ATTACK-RFI.conf",
  "waf-conf/coreruleset/rules/REQUEST-932-APPLICATION-ATTACK-RCE.conf",
  "waf-conf/coreruleset/rules/REQUEST-933-APPLICATION-ATTACK-PHP.conf",
  "waf-conf/coreruleset/rules/REQUEST-934-APPLICATION-ATTACK-GENERIC.conf",
  "waf-conf/coreruleset/rules/REQUEST-941-APPLICATION-ATTACK-XSS.conf",
  "waf-conf/coreruleset/rules/REQUEST-942-APPLICATION-ATTACK-SQLI.conf",
  "waf-conf/coreruleset/rules/REQUEST-943-APPLICATION-ATTACK-SESSION-FIXATION.conf",
  "waf-conf/coreruleset/rules/REQUEST-944-APPLICATION-ATTACK-JAVA.conf",
  "waf-conf/coreruleset/rules/REQUEST-949-BLOCKING-EVALUATION.conf",
  "waf-conf/coreruleset/rules/RESPONSE-950-DATA-LEAKAGES.conf",
  "waf-conf/coreruleset/rules/RESPONSE-951-DATA-LEAKAGES-SQL.conf",
  "waf-conf/coreruleset/rules/RESPONSE-952-DATA-LEAKAGES-JAVA.conf",
  "waf-conf/coreruleset/rules/RESPONSE-953-DATA-LEAKAGES-PHP.conf",
  "waf-conf/coreruleset/rules/RESPONSE-954-DATA-LEAKAGES-IIS.conf",
  "waf-conf/coreruleset/rules/RESPONSE-955-WEB-SHELLS.conf",
  "waf-conf/base/after.conf"});
// clang-format on

TEST_F(ParserTest, Empty) {
  const std::string directive = R"()";

  Antlr4::Parser parser;
  std::string error = parser.load(directive);
  if (!error.empty()) {
    std::cout << error << std::endl;
  }

  ASSERT_TRUE(error.empty());
}

TEST_F(ParserTest, Comment) {
  const std::string directive = R"(# This is comment1
  # This is comment2
  # This is comment3)";

  Antlr4::Parser parser;
  std::string error = parser.load(directive);
  if (!error.empty()) {
    std::cout << error << std::endl;
  }

  ASSERT_TRUE(error.empty());
}

TEST_F(ParserTest, Include) {
  const std::string directive = R"(# Test include directive
  Include "test/test_data/include_test.conf"
  )";

  Antlr4::Parser parser;
  std::string error = parser.load(directive);
  if (!error.empty()) {
    std::cout << error << std::endl;
  }

  ASSERT_TRUE(error.empty());
}

TEST_F(ParserTest, EngineConfig) {
  {
    const std::string directive = R"(# Test engine config
  SecRequestBodyAccess On
  SecResponseBodyAccess On
  SecRuleEngine On
  SecTmpSaveUploadedFiles On
  SecUploadKeepFiles On
  SecXmlExternalEntity On
  )";

    Antlr4::Parser parser;
    std::string error = parser.load(directive);
    ASSERT_TRUE(error.empty());

    const auto& engine_config = parser.engineConfig();
    ASSERT_EQ(engine_config.is_request_body_access_, Antlr4::Parser::EngineConfig::Option::On);
    ASSERT_EQ(engine_config.is_response_body_access_, Antlr4::Parser::EngineConfig::Option::On);
    ASSERT_EQ(engine_config.is_rule_engine_, Antlr4::Parser::EngineConfig::Option::On);
    ASSERT_EQ(engine_config.is_tmp_save_uploaded_files_, Antlr4::Parser::EngineConfig::Option::On);
    ASSERT_EQ(engine_config.is_upload_keep_files_, Antlr4::Parser::EngineConfig::Option::On);
    ASSERT_EQ(engine_config.is_xml_external_entity_, Antlr4::Parser::EngineConfig::Option::On);
  }

  {
    const std::string directive = R"(# Test engine config
  SecRequestBodyAccess Off
  SecResponseBodyAccess Off
  SecRuleEngine Off
  SecTmpSaveUploadedFiles Off
  SecUploadKeepFiles Off
  SecXmlExternalEntity Off
  )";

    Antlr4::Parser parser;
    std::string error = parser.load(directive);
    ASSERT_TRUE(error.empty());

    const auto& engine_config = parser.engineConfig();
    ASSERT_EQ(engine_config.is_request_body_access_, Antlr4::Parser::EngineConfig::Option::Off);
    ASSERT_EQ(engine_config.is_response_body_access_, Antlr4::Parser::EngineConfig::Option::Off);
    ASSERT_EQ(engine_config.is_rule_engine_, Antlr4::Parser::EngineConfig::Option::Off);
    ASSERT_EQ(engine_config.is_tmp_save_uploaded_files_, Antlr4::Parser::EngineConfig::Option::Off);
    ASSERT_EQ(engine_config.is_upload_keep_files_, Antlr4::Parser::EngineConfig::Option::Off);
    ASSERT_EQ(engine_config.is_xml_external_entity_, Antlr4::Parser::EngineConfig::Option::Off);
  }

  {
    const std::string directive = R"(# Test engine config
  SecRequestBodyAccess Off
  SecResponseBodyAccess Off
  SecRuleEngine DetectionOnly
  SecTmpSaveUploadedFiles Off
  SecUploadKeepFiles Off
  SecXmlExternalEntity Off
  )";

    Antlr4::Parser parser;
    std::string error = parser.load(directive);
    ASSERT_TRUE(error.empty());

    const auto& engine_config = parser.engineConfig();
    ASSERT_EQ(engine_config.is_request_body_access_, Antlr4::Parser::EngineConfig::Option::Off);
    ASSERT_EQ(engine_config.is_response_body_access_, Antlr4::Parser::EngineConfig::Option::Off);
    ASSERT_EQ(engine_config.is_rule_engine_, Antlr4::Parser::EngineConfig::Option::DetectionOnly);
    ASSERT_EQ(engine_config.is_tmp_save_uploaded_files_, Antlr4::Parser::EngineConfig::Option::Off);
    ASSERT_EQ(engine_config.is_upload_keep_files_, Antlr4::Parser::EngineConfig::Option::Off);
    ASSERT_EQ(engine_config.is_xml_external_entity_, Antlr4::Parser::EngineConfig::Option::Off);
  }

  {
    const std::string directive = R"(# Test engine config
  SecRequestBodyAccess DetectionOnly
  )";

    Antlr4::Parser parser;
    std::string error = parser.load(directive);
    ASSERT_TRUE(!error.empty());
  }

  {
    const std::string directive = R"(# Test engine config
  SecResponseBodyAccess DetectionOnly
  )";

    Antlr4::Parser parser;
    std::string error = parser.load(directive);
    ASSERT_TRUE(!error.empty());
  }

  {
    const std::string directive = R"(# Test engine config
  SecTmpSaveUploadedFiles DetectionOnly
  )";

    Antlr4::Parser parser;
    std::string error = parser.load(directive);
    ASSERT_TRUE(!error.empty());
  }

  {
    const std::string directive = R"(# Test engine config
  SecUploadKeepFiles DetectionOnly
  )";

    Antlr4::Parser parser;
    std::string error = parser.load(directive);
    ASSERT_TRUE(!error.empty());
  }

  {
    const std::string directive = R"(# Test engine config
  SecXmlExternalEntity DetectionOnly
  )";

    Antlr4::Parser parser;
    std::string error = parser.load(directive);
    ASSERT_TRUE(!error.empty());
  }
}

TEST_F(ParserTest, RuleDirective) {
  const std::string rule_directive = R"(SecRule ARGS_GET|ARGS_POST:hello "world" "id:1")";
  Antlr4::Parser parser;
  std::string error = parser.load(rule_directive);
  ASSERT_TRUE(error.empty());

  // variables pool
  EXPECT_EQ(parser.rules().size(), 1);
  auto& rule_var_pool = ruleVariablePool(*parser.rules().back());
  ASSERT_EQ(rule_var_pool.size(), 2);
  EXPECT_EQ(rule_var_pool[0]->fullName(), "ARGS_GET");
  EXPECT_EQ(rule_var_pool[0]->mainName(), "ARGS_GET");
  EXPECT_EQ(rule_var_pool[0]->subName(), "");
  EXPECT_EQ(rule_var_pool[1]->fullName(), "ARGS_POST:hello");
  EXPECT_EQ(rule_var_pool[1]->mainName(), "ARGS_POST");
  EXPECT_EQ(rule_var_pool[1]->subName(), "hello");

  // variables map
  auto& rule_var_map = ruleVariableMap(*parser.rules().back());
  {
    auto iter = rule_var_map.find("ARGS_GET");
    ASSERT_TRUE(iter != rule_var_map.end());
    EXPECT_EQ(&iter->second, rule_var_pool[0].get());
  }
  {
    auto iter = rule_var_map.find("ARGS_POST:hello");
    ASSERT_TRUE(iter != rule_var_map.end());
    EXPECT_EQ(&iter->second, rule_var_pool[1].get());
  }
}
} // namespace SrSecurity