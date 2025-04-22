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
#include <format>
#include <functional>
#include <string>
#include <vector>

#include <gtest/gtest.h>
#include <stdlib.h>

#include "action/actions_include.h"
#include "antlr4/parser.h"
#include "engine.h"
#include "transformation/transform_include.h"
#include "variable/variables_include.h"

namespace SrSecurity {
namespace Parser {
class RuleActionParseTest : public testing::Test {
private:
  // Use for specific the main thread id, so that the ASSERT_IS_MAIN_THREAD macro can work
  // correctly in the test.
  Engine main_thread_id_init_helper_;
};

TEST_F(RuleActionParseTest, NoAction) {
  const std::string rule_directive =
      R"(SecRule ARGS:aaa|ARGS:bbb "foo"
      SecRule ARGS:aaa|ARGS:bbb "bar")";

  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());

  EXPECT_EQ(parser.rules().size(), 2);
  EXPECT_EQ(parser.rules().front()->actions().size(), 0);
  EXPECT_EQ(parser.rules().back()->actions().size(), 0);
}

TEST_F(RuleActionParseTest, ActionSetVar) {
  const std::string rule_directive =
      R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,setvar:'tx.score',msg:'aaa'")";

  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());

  auto& actions = parser.rules().back()->actions();
  EXPECT_EQ(actions.size(), 1);
  EXPECT_EQ(std::string_view(actions.back()->name()), "setvar");
}

TEST_F(RuleActionParseTest, ActionSetVarWithNoSigleQuote) {
  const std::string rule_directive =
      R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,setvar:tx.score,msg:'aaa'")";

  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());

  auto& actions = parser.rules().back()->actions();
  EXPECT_EQ(actions.size(), 1);
  EXPECT_EQ(std::string_view(actions.back()->name()), "setvar");
}

TEST_F(RuleActionParseTest, ActionSetEnv) {
  const std::string rule_directive =
      R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,setenv:'var1=hello',msg:'aaa bbb'")";

  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());

  auto& actions = parser.rules().back()->actions();
  EXPECT_EQ(actions.size(), 1);
  EXPECT_EQ(std::string_view(actions.back()->name()), "setenv");
}

TEST_F(RuleActionParseTest, ActionSetRsc) {
  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,setrsc:'this is rsc',msg:'aaa'")";

    Antlr4::Parser parser;
    auto result = parser.load(rule_directive);
    ASSERT_TRUE(result.has_value());
    ASSERT_TRUE(result.has_value());
    auto& actions = parser.rules().back()->actions();
    EXPECT_EQ(actions.size(), 1);
    EXPECT_EQ(std::string_view(actions.back()->name()), "setrsc");
  }

  // Macro expansion
  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,setrsc:%{tx.0},msg:'aaa'")";

    Antlr4::Parser parser;
    auto result = parser.load(rule_directive);
    ASSERT_TRUE(result.has_value());

    auto& actions = parser.rules().back()->actions();
    EXPECT_EQ(actions.size(), 1);
    EXPECT_EQ(std::string_view(actions.back()->name()), "setrsc");
  }
}

TEST_F(RuleActionParseTest, ActionSetSid) {
  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,setsid:'this is sid',msg:'aaa'")";
    Antlr4::Parser parser;
    auto result = parser.load(rule_directive);
    ASSERT_TRUE(result.has_value());
    auto& actions = parser.rules().back()->actions();
    EXPECT_EQ(actions.size(), 1);
    EXPECT_EQ(std::string_view(actions.back()->name()), "setsid");
  }

  // Macro expansion
  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,setsid:%{tx.0},msg:'aaa'")";
    Antlr4::Parser parser;
    auto result = parser.load(rule_directive);
    ASSERT_TRUE(result.has_value());
    auto& actions = parser.rules().back()->actions();
    EXPECT_EQ(actions.size(), 1);
    EXPECT_EQ(std::string_view(actions.back()->name()), "setsid");
  }
}

TEST_F(RuleActionParseTest, ActionSetUid) {
  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,setuid:'this is uid',msg:'aaa'")";
    Antlr4::Parser parser;
    auto result = parser.load(rule_directive);
    ASSERT_TRUE(result.has_value());
    auto& actions = parser.rules().back()->actions();
    EXPECT_EQ(actions.size(), 1);
    EXPECT_EQ(std::string_view(actions.back()->name()), "setuid");
  }

  // Macro expansion
  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,setuid:%{tx.0},msg:'aaa'")";
    Antlr4::Parser parser;
    auto result = parser.load(rule_directive);
    ASSERT_TRUE(result.has_value());
    auto& actions = parser.rules().back()->actions();
    EXPECT_EQ(actions.size(), 1);
    EXPECT_EQ(std::string_view(actions.back()->name()), "setuid");
  }
}

TEST_F(RuleActionParseTest, ActionTransformation) {
  const std::string rule_directive =
      R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,auditlog,t:none,t:hexDecode,msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());
  auto& transforms = parser.rules().back()->transforms();
  EXPECT_TRUE(parser.rules().back()->isIgnoreDefaultTransform());
  EXPECT_NE(nullptr, dynamic_cast<Transformation::HexDecode*>(transforms[0].get()));

  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,auditlog,t:none,t:hexDecode123,msg:'aaa'")";
    Antlr4::Parser parser;
    auto result = parser.load(rule_directive);
    ASSERT_TRUE(!result.has_value());
  }
}

TEST_F(RuleActionParseTest, ActionAuditLog) {
  const std::string rule_directive = R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,auditlog,msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());
  EXPECT_TRUE(parser.rules().back()->auditLog());
}

TEST_F(RuleActionParseTest, ActionLog) {
  const std::string rule_directive = R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,log,msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());
  EXPECT_TRUE(parser.rules().back()->log());
}

TEST_F(RuleActionParseTest, ActionNoAuditLog) {
  const std::string rule_directive =
      R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,noauditlog,msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());
  EXPECT_FALSE(parser.rules().back()->auditLog().value_or(true));
}

TEST_F(RuleActionParseTest, ActionNoLog) {
  const std::string rule_directive = R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,nolog,msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());
  EXPECT_FALSE(parser.rules().back()->log().value_or(true));
}

TEST_F(RuleActionParseTest, ActionCapture) {
  const std::string rule_directive = R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,capture,msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());
  EXPECT_TRUE(parser.rules().back()->capture());
}

TEST_F(RuleActionParseTest, ActionMultiMatch) {
  const std::string rule_directive =
      R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,multiMatch,msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());
  EXPECT_TRUE(parser.rules().back()->multiMatch());
}

TEST_F(RuleActionParseTest, ActionAllow) {
  const std::string rule_directive = R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,allow,msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(parser.rules().back()->disruptive(), Rule::Disruptive::ALLOW);
}

TEST_F(RuleActionParseTest, ActionBlock) {
  const std::string rule_directive = R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,block,msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(parser.rules().back()->disruptive(), Rule::Disruptive::BLOCK);
}

TEST_F(RuleActionParseTest, ActionDeny) {
  const std::string rule_directive = R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,deny,msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(parser.rules().back()->disruptive(), Rule::Disruptive::DENY);
}

TEST_F(RuleActionParseTest, ActionDrop) {
  const std::string rule_directive = R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,drop,msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(parser.rules().back()->disruptive(), Rule::Disruptive::DROP);
}

TEST_F(RuleActionParseTest, ActionPass) {
  const std::string rule_directive = R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,pass,msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(parser.rules().back()->disruptive(), Rule::Disruptive::PASS);
}

TEST_F(RuleActionParseTest, ActionRedirect) {
  const std::string rule_directive =
      R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,redirect:http://www.srhino.com,msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(parser.rules().back()->disruptive(), Rule::Disruptive::REDIRECT);
  EXPECT_EQ(parser.rules().back()->redirect(), "http://www.srhino.com");
}

TEST_F(RuleActionParseTest, ActionStatus) {
  const std::string rule_directive =
      R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,status:500,msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(parser.rules().back()->status(), "500");
}

TEST_F(RuleActionParseTest, ActionXmlns) {
  const std::string rule_directive =
      R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,xmlns:xsd=http://www.w3.org/2001/XMLSchema,msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(parser.rules().back()->xmlns(), "xsd=http://www.w3.org/2001/XMLSchema");
}

TEST_F(RuleActionParseTest, ActionCtlAuditEngine) {
  const std::string rule_directive =
      R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,ctl:auditEngine=On,msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());

  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,ctl:auditEngine=Off,msg:'aaa'")";
    Antlr4::Parser parser;
    auto result = parser.load(rule_directive);
    ASSERT_TRUE(result.has_value());
  }

  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,ctl:auditEngine=RelevantOnly,msg:'aaa'")";
    Antlr4::Parser parser;
    auto result = parser.load(rule_directive);
    ASSERT_TRUE(result.has_value());
  }

  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,ctl:auditEngine=asdfasdf,msg:'aaa'")";
    Antlr4::Parser parser;
    auto result = parser.load(rule_directive);
    ASSERT_TRUE(!result.has_value());
  }
}

TEST_F(RuleActionParseTest, ActionCtlAuditLogParts) {
  const std::string rule_directive =
      R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,ctl:auditLogParts=+ABCDEF,msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());

  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,ctl:auditLogParts=-ABCDEF,msg:'aaa'")";
    Antlr4::Parser parser;
    auto result = parser.load(rule_directive);
    ASSERT_TRUE(result.has_value());
  }

  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,ctl:auditLogParts=+ABCDEFL,msg:'aaa'")";
    Antlr4::Parser parser;
    auto result = parser.load(rule_directive);
    ASSERT_FALSE(result.has_value());
  }
}

TEST_F(RuleActionParseTest, ActionCtlRequestBodyAccess) {
  const std::string rule_directive =
      R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,ctl:requestBodyAccess=On,msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());

  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,ctl:requestBodyAccess=Off,msg:'aaa'")";
    Antlr4::Parser parser;
    auto result = parser.load(rule_directive);
    ASSERT_TRUE(result.has_value());
  }

  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,ctl:requestBodyAccess=Hi,msg:'aaa'")";
    Antlr4::Parser parser;
    auto result = parser.load(rule_directive);
    ASSERT_FALSE(result.has_value());
  }
}

TEST_F(RuleActionParseTest, ActionCtlRequestBodyProcessor) {
  const std::string rule_directive =
      R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,ctl:requestBodyProcessor=XML,msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());

  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,ctl:requestBodyProcessor=JSON,msg:'aaa'")";
    Antlr4::Parser parser;
    auto result = parser.load(rule_directive);
    ASSERT_TRUE(result.has_value());
  }

  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,ctl:requestBodyProcessor=Hi,msg:'aaa'")";
    Antlr4::Parser parser;
    auto result = parser.load(rule_directive);
    ASSERT_FALSE(result.has_value());
  }
}

TEST_F(RuleActionParseTest, ActionCtlRuleEngine) {
  const std::string rule_directive =
      R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,ctl:ruleEngine=On,msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());

  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,ctl:ruleEngine=Off,msg:'aaa'")";
    Antlr4::Parser parser;
    auto result = parser.load(rule_directive);
    ASSERT_TRUE(result.has_value());
  }

  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,ctl:ruleEngine=DetectionOnly,msg:'aaa'")";
    Antlr4::Parser parser;
    auto result = parser.load(rule_directive);
    ASSERT_TRUE(result.has_value());
  }

  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,ctl:ruleEngine=Hi,msg:'aaa'")";
    Antlr4::Parser parser;
    auto result = parser.load(rule_directive);
    ASSERT_FALSE(result.has_value());
  }
}

TEST_F(RuleActionParseTest, ActionCtlRuleRemoveById) {
  const std::string rule_directive =
      R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,ctl:ruleRemoveById=123,msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());

  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,ctl:ruleRemoveById=222-333,msg:'aaa'")";
    Antlr4::Parser parser;
    auto result = parser.load(rule_directive);
    ASSERT_TRUE(result.has_value());
  }
}

TEST_F(RuleActionParseTest, ActionCtlRuleRemoveByTag) {
  const std::string rule_directive =
      R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,ctl:ruleRemoveByTag=foo,msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());
}

TEST_F(RuleActionParseTest, ActionCtlRuleRemoveTargetById) {
  const std::string rule_directive =
      R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,ctl:ruleRemoveTargetById=123;ARGS:foo|ARGS:bar,msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());
}

TEST_F(RuleActionParseTest, ActionCtlRuleRemoveTargetByTag) {
  const std::string rule_directive =
      R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,ctl:ruleRemoveTargetByTag=foo;ARGS:foo|ARGS:bar,msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());
}

TEST_F(RuleActionParseTest, ActionChain) {
  const std::string rule_directive =
      R"(SecRule ARGS:aaa|ARGS:bbb "foo" "id:1,ctl:ruleRemoveTargetByTag=foo;ARGS:foo|ARGS:bar,msg:'aaa',chain"
SecRule ARGS_GET|ARGS_POST:foo|!ARGS_GET:foo|&ARGS "bar" "id:2,tag:'foo',msg:'bar'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(parser.rules().size(), 1);

  // Variables pool
  auto& chain_rule = parser.rules().back()->backChainRule();
  auto& rule_var_pool = chain_rule->variables();
  ASSERT_EQ(rule_var_pool.size(), 3);
  EXPECT_NE(nullptr, dynamic_cast<Variable::ArgsGet*>(rule_var_pool[0].get()));
  EXPECT_EQ(rule_var_pool[0]->subName(), "");
  EXPECT_FALSE(rule_var_pool[0]->isCounter());
  EXPECT_FALSE(rule_var_pool[0]->isNot());

  EXPECT_NE(nullptr, dynamic_cast<Variable::ArgsPost*>(rule_var_pool[1].get()));
  EXPECT_EQ(rule_var_pool[1]->subName(), "foo");
  EXPECT_FALSE(rule_var_pool[1]->isCounter());
  EXPECT_FALSE(rule_var_pool[1]->isNot());

  EXPECT_NE(nullptr, dynamic_cast<Variable::Args*>(rule_var_pool[2].get()));
  EXPECT_EQ(rule_var_pool[2]->subName(), "");
  EXPECT_TRUE(rule_var_pool[2]->isCounter());
  EXPECT_FALSE(rule_var_pool[2]->isNot());

  auto& except_var_pool = chain_rule->exceptVariables();
  ASSERT_EQ(except_var_pool.size(), 1);
  EXPECT_NE(nullptr, dynamic_cast<Variable::ArgsGet*>(except_var_pool[0].get()));
  EXPECT_EQ(except_var_pool[0]->subName(), "foo");
  EXPECT_FALSE(except_var_pool[0]->isCounter());
  EXPECT_TRUE(except_var_pool[0]->isNot());

  // variables map
  auto& rule_var_index = chain_rule->variablesIndex();
  {
    auto iter = rule_var_index.find({"ARGS_GET", ""});
    ASSERT_TRUE(iter != rule_var_index.end());
    EXPECT_EQ(&iter->second, rule_var_pool[0].get());
  }
  {
    auto iter = rule_var_index.find({"ARGS_POST", "foo"});
    ASSERT_TRUE(iter != rule_var_index.end());
    EXPECT_EQ(&iter->second, rule_var_pool[1].get());
  }

  // operator
  auto& rule_operator = chain_rule->getOperator();
  EXPECT_EQ(rule_operator->name(), std::string("rx"));
  EXPECT_EQ(rule_operator->literalValue(), "bar");
}

TEST_F(RuleActionParseTest, ActionInitCol) {
  const std::string rule_directive =
      R"(SecRule ARGS:aaa|ARGS:bbb "foo" "id:1,initcol:global=global,initcol:ip=%{REMOTE_ADDR}")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(parser.rules().size(), 1);
  auto& actions = parser.rules().back()->actions();
  EXPECT_EQ(actions.size(), 2);
  EXPECT_NE(nullptr, dynamic_cast<Action::InitCol*>(actions.front().get()));
}

TEST_F(RuleActionParseTest, ActionSkipAfter) {
  const std::string rule_directive =
      R"(SecRule ARGS:aaa|ARGS:bbb "foo" "id:1,skipAfter:hi,msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(parser.rules().back()->skipAfter(), "hi");
}

TEST_F(RuleActionParseTest, ActionSkip) {
  const std::string rule_directive = R"(SecRule ARGS:aaa|ARGS:bbb "foo" "id:1,skip:3,msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(parser.rules().back()->skip(), 3);
}

TEST_F(RuleActionParseTest, ActionServerity) {
  const std::string rule_directive =
      R"(SecRule ARGS:aaa|ARGS:bbb "foo" "id:1,severity:2,msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(static_cast<uint32_t>(parser.rules().back()->severity()), 2);

  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "foo" "id:1,severity:8,msg:'aaa'")";
    Antlr4::Parser parser;
    auto result = parser.load(rule_directive);
    ASSERT_FALSE(result.has_value());
  }

  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "foo" "id:1,severity:'EMERGENCY',msg:'aaa'")";
    Antlr4::Parser parser;
    auto result = parser.load(rule_directive);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(static_cast<uint32_t>(parser.rules().back()->severity()), 0);
  }

  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "foo" "id:1,severity:'ALERT',msg:'aaa'")";
    Antlr4::Parser parser;
    auto result = parser.load(rule_directive);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(static_cast<uint32_t>(parser.rules().back()->severity()), 1);
  }

  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "foo" "id:1,severity:'CRITICAL',msg:'aaa'")";
    Antlr4::Parser parser;
    auto result = parser.load(rule_directive);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(static_cast<uint32_t>(parser.rules().back()->severity()), 2);
  }

  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "foo" "id:1,severity:'ERROR',msg:'aaa'")";
    Antlr4::Parser parser;
    auto result = parser.load(rule_directive);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(static_cast<uint32_t>(parser.rules().back()->severity()), 3);
  }

  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "foo" "id:1,severity:'WARNING',msg:'aaa'")";
    Antlr4::Parser parser;
    auto result = parser.load(rule_directive);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(static_cast<uint32_t>(parser.rules().back()->severity()), 4);
  }

  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "foo" "id:1,severity:'NOTICE',msg:'aaa'")";
    Antlr4::Parser parser;
    auto result = parser.load(rule_directive);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(static_cast<uint32_t>(parser.rules().back()->severity()), 5);
  }

  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "foo" "id:1,severity:'INFO',msg:'aaa'")";
    Antlr4::Parser parser;
    auto result = parser.load(rule_directive);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(static_cast<uint32_t>(parser.rules().back()->severity()), 6);
  }

  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "foo" "id:1,severity:'DEBUG',msg:'aaa'")";
    Antlr4::Parser parser;
    auto result = parser.load(rule_directive);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(static_cast<uint32_t>(parser.rules().back()->severity()), 7);
  }

  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "foo" "id:1,severity:'HI',msg:'aaa'")";
    Antlr4::Parser parser;
    auto result = parser.load(rule_directive);
    ASSERT_FALSE(result.has_value());
  }
}

TEST_F(RuleActionParseTest, ActionIdWithString) {
  {
    const std::string rule_directive =
        R"(SecRule ARGS_GET|ARGS_POST:foo|!ARGS_GET:foo|&ARGS "bar" "id:'123abc',tag:'foo',msg:'bar'")";
    Antlr4::Parser parser;
    auto result = parser.load(rule_directive);

    // id must be a number
    ASSERT_FALSE(result.has_value());
  }

  const std::string rule_directive =
      R"(SecRule ARGS_GET|ARGS_POST:foo|!ARGS_GET:foo|&ARGS "bar" "id:'1',tag:'foo',msg:'bar'")";

  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());

  // Variables pool
  EXPECT_EQ(parser.rules().size(), 1);
  auto& rule_var_pool = parser.rules().back()->variables();
  ASSERT_EQ(rule_var_pool.size(), 3);
  EXPECT_NE(nullptr, dynamic_cast<Variable::ArgsGet*>(rule_var_pool[0].get()));
  EXPECT_EQ(rule_var_pool[0]->subName(), "");
  EXPECT_FALSE(rule_var_pool[0]->isCounter());
  EXPECT_FALSE(rule_var_pool[0]->isNot());

  EXPECT_NE(nullptr, dynamic_cast<Variable::ArgsPost*>(rule_var_pool[1].get()));
  EXPECT_EQ(rule_var_pool[1]->subName(), "foo");
  EXPECT_FALSE(rule_var_pool[1]->isCounter());
  EXPECT_FALSE(rule_var_pool[1]->isNot());

  EXPECT_NE(nullptr, dynamic_cast<Variable::Args*>(rule_var_pool[2].get()));
  EXPECT_EQ(rule_var_pool[2]->subName(), "");
  EXPECT_TRUE(rule_var_pool[2]->isCounter());
  EXPECT_FALSE(rule_var_pool[2]->isNot());

  auto& except_var_pool = parser.rules().back()->exceptVariables();
  ASSERT_EQ(except_var_pool.size(), 1);
  EXPECT_NE(nullptr, dynamic_cast<Variable::ArgsGet*>(except_var_pool[0].get()));
  EXPECT_EQ(except_var_pool[0]->subName(), "foo");
  EXPECT_FALSE(except_var_pool[0]->isCounter());
  EXPECT_TRUE(except_var_pool[0]->isNot());

  // variables map
  auto& rule_var_index = parser.rules().back()->variablesIndex();
  {
    auto iter = rule_var_index.find({"ARGS_GET", ""});
    ASSERT_TRUE(iter != rule_var_index.end());
    EXPECT_EQ(&iter->second, rule_var_pool[0].get());
  }
  {
    auto iter = rule_var_index.find({"ARGS_POST", "foo"});
    ASSERT_TRUE(iter != rule_var_index.end());
    EXPECT_EQ(&iter->second, rule_var_pool[1].get());
  }

  // operator
  auto& rule_operator = parser.rules().back()->getOperator();
  EXPECT_EQ(rule_operator->name(), std::string("rx"));
  EXPECT_EQ(rule_operator->literalValue(), "bar");
}

TEST_F(RuleActionParseTest, ActionMsgWithMacro) {
  const std::string rule_directive =
      R"(SecRule ARGS_GET|ARGS_POST:foo|!ARGS_GET:foo|&ARGS "bar" "id:'111',tag:'foo',msg:'foo: %{tx.foo} bar: %{tx.bar}'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);

  ASSERT_TRUE(result.has_value());

  EXPECT_TRUE(parser.rules().back()->msg().empty());
}

TEST_F(RuleActionParseTest, ActionLogData) {
  const std::string rule_directive =
      R"(SecRule ARGS:aaa|ARGS:bbb "foo" "id:1,logdata:'this is logdata',msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(parser.rules().back()->logdata(), "this is logdata");
}

TEST_F(RuleActionParseTest, ActionLogDataWithMacro) {
  const std::string rule_directive =
      R"(SecRule ARGS:aaa|ARGS:bbb "foo" "id:1,logdata:'foo: %{tx.foo} bar: %{tx.bar}',msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());

  EXPECT_TRUE(parser.rules().back()->logdata().empty());
}
} // namespace Parser
} // namespace SrSecurity