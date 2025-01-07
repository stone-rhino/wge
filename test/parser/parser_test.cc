#include <functional>
#include <string>
#include <vector>

#include <gtest/gtest.h>

#include "antlr4/parser.h"

namespace SrSecurity {
class ParserTest : public testing::Test {
public:
  const std::vector<std::unique_ptr<Variable::VariableBase>>&
  getRuleVariablePool(Rule& rule) const {
    return rule.variables_pool_;
  }

  const std::unordered_map<std::string_view, Variable::VariableBase&>&
  getRuleVariableIndex(Rule& rule) const {
    return rule.variables_index_by_full_name_;
  }

  const std::unique_ptr<Operator::OperatorBase>& getRuleOperator(Rule& rule) {
    return rule.operator_;
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
  const std::string rule_directive =
      R"(SecRule ARGS_GET|ARGS_POST:foo|!ARGS_GET:foo|&ARGS "bar" "id:1,tag:foo,msg:bar")";
  Antlr4::Parser parser;
  std::string error = parser.load(rule_directive);
  ASSERT_TRUE(error.empty());

  // variables pool
  EXPECT_EQ(parser.rules().size(), 1);
  auto& rule_var_pool = getRuleVariablePool(*parser.rules().back());
  ASSERT_EQ(rule_var_pool.size(), 4);
  EXPECT_EQ(rule_var_pool[0]->fullName(), "ARGS_GET");
  EXPECT_EQ(rule_var_pool[0]->mainName(), "ARGS_GET");
  EXPECT_EQ(rule_var_pool[0]->subName(), "");
  EXPECT_FALSE(rule_var_pool[0]->isCounter());
  EXPECT_FALSE(rule_var_pool[0]->isNot());

  EXPECT_EQ(rule_var_pool[1]->fullName(), "ARGS_POST:foo");
  EXPECT_EQ(rule_var_pool[1]->mainName(), "ARGS_POST");
  EXPECT_EQ(rule_var_pool[1]->subName(), "foo");
  EXPECT_FALSE(rule_var_pool[1]->isCounter());
  EXPECT_FALSE(rule_var_pool[1]->isNot());

  EXPECT_EQ(rule_var_pool[2]->fullName(), "ARGS_GET:foo");
  EXPECT_EQ(rule_var_pool[2]->mainName(), "ARGS_GET");
  EXPECT_EQ(rule_var_pool[2]->subName(), "foo");
  EXPECT_FALSE(rule_var_pool[2]->isCounter());
  EXPECT_TRUE(rule_var_pool[2]->isNot());

  EXPECT_EQ(rule_var_pool[3]->fullName(), "ARGS");
  EXPECT_EQ(rule_var_pool[3]->mainName(), "ARGS");
  EXPECT_EQ(rule_var_pool[3]->subName(), "");
  EXPECT_TRUE(rule_var_pool[3]->isCounter());
  EXPECT_FALSE(rule_var_pool[3]->isNot());

  // variables map
  auto& rule_var_index = getRuleVariableIndex(*parser.rules().back());
  {
    auto iter = rule_var_index.find("ARGS_GET");
    ASSERT_TRUE(iter != rule_var_index.end());
    EXPECT_EQ(&iter->second, rule_var_pool[0].get());
  }
  {
    auto iter = rule_var_index.find("ARGS_POST:foo");
    ASSERT_TRUE(iter != rule_var_index.end());
    EXPECT_EQ(&iter->second, rule_var_pool[1].get());
  }

  // operator
  auto& rule_operator = getRuleOperator(*parser.rules().back());
  EXPECT_EQ(rule_operator->name(), "rx");
  EXPECT_EQ(rule_operator->value(), "bar");
  EXPECT_EQ(rule_operator->regexExpr(), "bar");
}

TEST_F(ParserTest, RuleRemoveById) {
  const std::string rule_directive = R"(SecRule ARGS "bar" "id:1,tag:tag1,msg:msg1"
  SecRule ARGS "bar" "id:2,tag:tag2,tag:tag3,msg:msg2"
  SecRule ARGS "bar" "id:3,tag:tag2,tag:tag3,msg:msg3"
  SecRule ARGS "bar" "id:4,tag:tag4,msg:msg4"
  SecRule ARGS "bar" "id:5,tag:tag5,msg:msg5"
  SecRule ARGS "bar" "id:6,tag:tag6,msg:msg6"
  SecRule ARGS "bar" "id:7,tag:tag6,msg:msg6"
  SecRule ARGS "bar" "id:8,tag:tag6,msg:msg6"
  SecRule ARGS "bar" "id:9,tag:tag6,msg:msg6"
  SecRule ARGS "bar" "id:10,tag:tag6,msg:msg6"
  )";

  Antlr4::Parser parser;
  std::string error = parser.load(rule_directive);
  ASSERT_TRUE(error.empty());

  auto& rules = parser.rules();
  EXPECT_EQ(rules.size(), 10);

  {
    const std::string rule_remove = R"(SecRuleRemoveById 1)";
    error = parser.load(rule_remove);
    ASSERT_TRUE(error.empty());
    EXPECT_EQ(rules.size(), 9);
    auto iter = std::find_if(rules.begin(), rules.end(),
                             [](const std::unique_ptr<Rule>& rule) { return rule->id() == 1; });
    EXPECT_EQ(iter, rules.end());
  }

  {
    const std::string rule_remove = R"(SecRuleRemoveById 2 3)";
    error = parser.load(rule_remove);
    ASSERT_TRUE(error.empty());
    EXPECT_EQ(rules.size(), 7);
    auto iter = std::find_if(rules.begin(), rules.end(), [](const std::unique_ptr<Rule>& rule) {
      return rule->id() == 2 || rule->id() == 3;
    });
    EXPECT_EQ(iter, rules.end());
  }

  {
    const std::string rule_remove = R"(SecRuleRemoveById 4 5 6-8)";
    error = parser.load(rule_remove);
    ASSERT_TRUE(error.empty());
    EXPECT_EQ(rules.size(), 2);
    auto iter = std::find_if(rules.begin(), rules.end(), [](const std::unique_ptr<Rule>& rule) {
      return rule->id() == 4 || rule->id() == 5 || rule->id() == 6 || rule->id() == 7 ||
             rule->id() == 8;
    });
    EXPECT_EQ(iter, rules.end());
  }
}

TEST_F(ParserTest, RuleRemoveByMsg) {
  const std::string rule_directive = R"(SecRule ARGS "bar" "id:1,tag:tag1,msg:msg1"
  SecRule ARGS "bar" "id:2,tag:tag2,tag:tag3,msg:msg2"
  SecRule ARGS "bar" "id:3,tag:tag2,tag:tag3,msg:msg3"
  SecRule ARGS "bar" "id:4,tag:tag4,msg:msg4"
  SecRule ARGS "bar" "id:5,tag:tag5,msg:msg5"
  SecRule ARGS "bar" "id:6,tag:tag6,msg:msg6"
  SecRule ARGS "bar" "id:7,tag:tag6,msg:msg6"
  SecRule ARGS "bar" "id:8,tag:tag6,msg:msg6"
  SecRule ARGS "bar" "id:9,tag:tag6,msg:msg6"
  SecRule ARGS "bar" "id:10,tag:tag6,msg:msg6"
  )";

  Antlr4::Parser parser;
  std::string error = parser.load(rule_directive);
  ASSERT_TRUE(error.empty());

  auto& rules = parser.rules();
  EXPECT_EQ(rules.size(), 10);

  {
    const std::string rule_remove = R"(SecRuleRemoveByMsg "msg1")";
    error = parser.load(rule_remove);
    ASSERT_TRUE(error.empty());
    EXPECT_EQ(rules.size(), 9);
    auto iter = std::find_if(rules.begin(), rules.end(),
                             [](const std::unique_ptr<Rule>& rule) { return rule->id() == 1; });
    EXPECT_EQ(iter, rules.end());
  }

  {
    const std::string rule_remove = R"(SecRuleRemoveByMsg "msg6")";
    error = parser.load(rule_remove);
    ASSERT_TRUE(error.empty());
    EXPECT_EQ(rules.size(), 4);
    auto iter = std::find_if(rules.begin(), rules.end(), [](const std::unique_ptr<Rule>& rule) {
      return rule->id() == 6 || rule->id() == 7 || rule->id() == 8 || rule->id() == 9 ||
             rule->id() == 10;
    });
    EXPECT_EQ(iter, rules.end());
  }
}

TEST_F(ParserTest, RuleRemoveByTag) {
  const std::string rule_directive = R"(SecRule ARGS "bar" "id:1,tag:tag1,msg:msg1"
  SecRule ARGS "bar" "id:2,tag:tag2,tag:tag3,msg:msg2"
  SecRule ARGS "bar" "id:3,tag:tag2,tag:tag3,msg:msg3"
  SecRule ARGS "bar" "id:4,tag:tag4,msg:msg4"
  SecRule ARGS "bar" "id:5,tag:tag5,msg:msg5"
  SecRule ARGS "bar" "id:6,tag:tag6,msg:msg6"
  SecRule ARGS "bar" "id:7,tag:tag6,msg:msg6"
  SecRule ARGS "bar" "id:8,tag:tag6,msg:msg6"
  SecRule ARGS "bar" "id:9,tag:tag6,msg:msg6"
  SecRule ARGS "bar" "id:10,tag:tag6,msg:msg6"
  )";

  Antlr4::Parser parser;
  std::string error = parser.load(rule_directive);
  ASSERT_TRUE(error.empty());

  auto& rules = parser.rules();
  EXPECT_EQ(rules.size(), 10);

  {
    const std::string rule_remove = R"(SecRuleRemoveByTag "tag1")";
    error = parser.load(rule_remove);
    ASSERT_TRUE(error.empty());
    EXPECT_EQ(rules.size(), 9);
    auto iter = std::find_if(rules.begin(), rules.end(),
                             [](const std::unique_ptr<Rule>& rule) { return rule->id() == 1; });
    EXPECT_EQ(iter, rules.end());
  }

  {
    const std::string rule_remove = R"(SecRuleRemoveByTag "tag2")";
    error = parser.load(rule_remove);
    ASSERT_TRUE(error.empty());
    EXPECT_EQ(rules.size(), 7);
    auto iter = std::find_if(rules.begin(), rules.end(), [](const std::unique_ptr<Rule>& rule) {
      return rule->id() == 2 || rule->id() == 3;
    });
    EXPECT_EQ(iter, rules.end());
  }

  {
    const std::string rule_remove = R"(SecRuleRemoveByTag "tag6")";
    error = parser.load(rule_remove);
    ASSERT_TRUE(error.empty());
    EXPECT_EQ(rules.size(), 2);
    auto iter = std::find_if(rules.begin(), rules.end(), [](const std::unique_ptr<Rule>& rule) {
      return rule->id() == 6 || rule->id() == 7 || rule->id() == 8 || rule->id() == 9 ||
             rule->id() == 10;
    });
    EXPECT_EQ(iter, rules.end());
  }
}

TEST_F(ParserTest, RuleUpdateActionById) {
  const std::string rule_directive = R"(SecRule ARGS "bar" "id:1,tag:tag1,msg:msg1")";

  Antlr4::Parser parser;
  std::string error = parser.load(rule_directive);
  ASSERT_TRUE(error.empty());
  EXPECT_EQ(parser.rules().back()->msg(), "msg1");

  {
    const std::string rule_update = R"(SecRuleUpdateActionById 1 "msg:msg2")";
    error = parser.load(rule_update);
    ASSERT_TRUE(error.empty());
    EXPECT_EQ(parser.rules().back()->msg(), "msg2");
  }

  {
    auto& tags = parser.rules().back()->tags();
    EXPECT_NE(tags.find("tag1"), tags.end());
    EXPECT_EQ(tags.find("tag2"), tags.end());
    EXPECT_EQ(tags.find("tag3"), tags.end());

    const std::string rule_update = R"(SecRuleUpdateActionById 1 "msg:msg3,tag:tag2,tag:tag3")";
    error = parser.load(rule_update);
    ASSERT_TRUE(error.empty());
    EXPECT_EQ(parser.rules().back()->msg(), "msg3");
    EXPECT_EQ(tags.find("tag1"), tags.end());
    EXPECT_NE(tags.find("tag2"), tags.end());
    EXPECT_NE(tags.find("tag3"), tags.end());
  }
}

TEST_F(ParserTest, RuleUpdateTargetById) {
  const std::string rule_directive = R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,tag:tag1,msg:msg1")";

  Antlr4::Parser parser;
  std::string error = parser.load(rule_directive);
  ASSERT_TRUE(error.empty());
  auto& variable_index = getRuleVariableIndex(*parser.rules().back());
  EXPECT_NE(variable_index.find("ARGS:aaa"), variable_index.end());
  EXPECT_NE(variable_index.find("ARGS:bbb"), variable_index.end());
  EXPECT_FALSE(variable_index.find("ARGS:aaa")->second.isNot());
  EXPECT_FALSE(variable_index.find("ARGS:bbb")->second.isNot());

  {
    const std::string rule_update = R"(SecRuleUpdateTargetById 1 ARGS:ccc)";
    error = parser.load(rule_update);
    ASSERT_TRUE(error.empty());
    EXPECT_NE(variable_index.find("ARGS:ccc"), variable_index.end());
  }

  {
    const std::string rule_update = R"(SecRuleUpdateTargetById 1 !ARGS:aaa|!ARGS:bbb)";
    error = parser.load(rule_update);
    ASSERT_TRUE(error.empty());
    EXPECT_NE(variable_index.find("ARGS:aaa"), variable_index.end());
    EXPECT_NE(variable_index.find("ARGS:bbb"), variable_index.end());
    EXPECT_NE(variable_index.find("ARGS:ccc"), variable_index.end());
    EXPECT_TRUE(variable_index.find("ARGS:aaa")->second.isNot());
    EXPECT_TRUE(variable_index.find("ARGS:bbb")->second.isNot());
  }
}

TEST_F(ParserTest, RuleUpdateTargetByMsg) {
  const std::string rule_directive = R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,tag:tag1,msg:msg1")";

  Antlr4::Parser parser;
  std::string error = parser.load(rule_directive);
  ASSERT_TRUE(error.empty());
  auto& variable_index = getRuleVariableIndex(*parser.rules().back());
  EXPECT_NE(variable_index.find("ARGS:aaa"), variable_index.end());
  EXPECT_NE(variable_index.find("ARGS:bbb"), variable_index.end());
  EXPECT_FALSE(variable_index.find("ARGS:aaa")->second.isNot());
  EXPECT_FALSE(variable_index.find("ARGS:bbb")->second.isNot());

  {
    const std::string rule_update = R"(SecRuleUpdateTargetByMsg "msg1" ARGS:ccc)";
    error = parser.load(rule_update);
    ASSERT_TRUE(error.empty());
    EXPECT_NE(variable_index.find("ARGS:ccc"), variable_index.end());
  }

  {
    const std::string rule_update = R"(SecRuleUpdateTargetByMsg "msg1" !ARGS:aaa|!ARGS:bbb)";
    error = parser.load(rule_update);
    ASSERT_TRUE(error.empty());
    EXPECT_NE(variable_index.find("ARGS:aaa"), variable_index.end());
    EXPECT_NE(variable_index.find("ARGS:bbb"), variable_index.end());
    EXPECT_NE(variable_index.find("ARGS:ccc"), variable_index.end());
    EXPECT_TRUE(variable_index.find("ARGS:aaa")->second.isNot());
    EXPECT_TRUE(variable_index.find("ARGS:bbb")->second.isNot());
  }
}

TEST_F(ParserTest, RuleUpdateTargetByTag) {
  const std::string rule_directive = R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,tag:tag1,msg:msg1")";

  Antlr4::Parser parser;
  std::string error = parser.load(rule_directive);
  ASSERT_TRUE(error.empty());
  auto& variable_index = getRuleVariableIndex(*parser.rules().back());
  EXPECT_NE(variable_index.find("ARGS:aaa"), variable_index.end());
  EXPECT_NE(variable_index.find("ARGS:bbb"), variable_index.end());
  EXPECT_FALSE(variable_index.find("ARGS:aaa")->second.isNot());
  EXPECT_FALSE(variable_index.find("ARGS:bbb")->second.isNot());

  {
    const std::string rule_update = R"(SecRuleUpdateTargetByTag "tag1" ARGS:ccc)";
    error = parser.load(rule_update);
    ASSERT_TRUE(error.empty());
    EXPECT_NE(variable_index.find("ARGS:ccc"), variable_index.end());
  }

  {
    const std::string rule_update = R"(SecRuleUpdateTargetByTag "tag1" !ARGS:aaa|!ARGS:bbb)";
    error = parser.load(rule_update);
    ASSERT_TRUE(error.empty());
    EXPECT_NE(variable_index.find("ARGS:aaa"), variable_index.end());
    EXPECT_NE(variable_index.find("ARGS:bbb"), variable_index.end());
    EXPECT_NE(variable_index.find("ARGS:ccc"), variable_index.end());
    EXPECT_TRUE(variable_index.find("ARGS:aaa")->second.isNot());
    EXPECT_TRUE(variable_index.find("ARGS:bbb")->second.isNot());
  }
}
} // namespace SrSecurity