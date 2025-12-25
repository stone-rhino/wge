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
#include "macro/macro_include.h"
#include "transformation/transform_include.h"
#include "variable/variables_include.h"

namespace Wge {
namespace Parser {
class RuleActionParseTest : public testing::Test {
private:
  // Use for specific the main thread id, so that the ASSERT_IS_MAIN_THREAD macro can work
  // correctly in the test.
  Engine main_thread_id_init_helper_;
};

TEST_F(RuleActionParseTest, NoAction) {
  const std::string rule_directive =
      R"(SecRule ARGS:aaa|ARGS:bbb "foo" "phase:1"
      SecRule ARGS:aaa|ARGS:bbb "bar" "phase:1")";

  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());

  EXPECT_EQ(parser.rules()[0].size(), 2);
  EXPECT_EQ(parser.rules()[0].front().actions().size(), 0);
  EXPECT_EQ(parser.rules()[0].back().actions().size(), 0);
}

TEST_F(RuleActionParseTest, ActionSetVar) {
  const std::string rule_directive =
      R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,setvar:'tx.score0',!setvar:'tx.score1',*setvar:'tx.score2', msg:'aaa'")";

  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());

  auto& all_actions = parser.rules()[0].back().actions();
  auto& matched_branch_actions = parser.rules()[0].back().matchedBranchActions();
  auto& unmatched_branch_actions = parser.rules()[0].back().unmatchedBranchActions();
  ASSERT_EQ(all_actions.size(), 3);
  ASSERT_EQ(matched_branch_actions.size(), 2);
  ASSERT_EQ(unmatched_branch_actions.size(), 2);

  const Action::SetVar* all_action0 = dynamic_cast<const Action::SetVar*>(all_actions[0].get());
  const Action::SetVar* all_action1 = dynamic_cast<const Action::SetVar*>(all_actions[1].get());
  const Action::SetVar* all_action2 = dynamic_cast<const Action::SetVar*>(all_actions[2].get());
  const Action::SetVar* matched_action0 =
      dynamic_cast<const Action::SetVar*>(matched_branch_actions[0]);
  const Action::SetVar* matched_action1 =
      dynamic_cast<const Action::SetVar*>(matched_branch_actions[1]);
  const Action::SetVar* unmatched_action0 =
      dynamic_cast<const Action::SetVar*>(unmatched_branch_actions[0]);
  const Action::SetVar* unmatched_action1 =
      dynamic_cast<const Action::SetVar*>(unmatched_branch_actions[1]);
  ASSERT_NE(all_action0, nullptr);
  ASSERT_NE(all_action1, nullptr);
  ASSERT_NE(all_action2, nullptr);
  ASSERT_NE(matched_action0, nullptr);
  ASSERT_NE(matched_action1, nullptr);
  ASSERT_NE(unmatched_action0, nullptr);
  ASSERT_NE(unmatched_action1, nullptr);

  EXPECT_EQ(all_action0, matched_action0);
  EXPECT_EQ(all_action1, unmatched_action0);
  EXPECT_EQ(all_action2, matched_action1);
  EXPECT_EQ(all_action2, unmatched_action1);

  EXPECT_EQ(std::string_view(all_actions[0]->name()), "setvar");
  EXPECT_EQ(std::string_view(all_actions[1]->name()), "setvar");
  EXPECT_EQ(std::string_view(all_actions[2]->name()), "setvar");
  EXPECT_EQ(all_action0->key(), "score0");
  EXPECT_EQ(all_action1->key(), "score1");
  EXPECT_EQ(all_action2->key(), "score2");
}

TEST_F(RuleActionParseTest, ActionSetVarWithNoSigleQuote) {
  const std::string rule_directive =
      R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,setvar:tx.score,msg:'aaa'")";

  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());

  auto& actions = parser.rules()[0].back().actions();
  EXPECT_EQ(actions.size(), 1);
  EXPECT_EQ(std::string_view(actions.back()->name()), "setvar");
}

TEST_F(RuleActionParseTest, ActionSetEnv) {
  const std::string rule_directive =
      R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,setenv:'score0=hello',!setenv:'score1=hello',*setenv:'score2=hello',msg:'aaa bbb'")";

  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());

  auto& all_actions = parser.rules()[0].back().actions();
  auto& matched_branch_actions = parser.rules()[0].back().matchedBranchActions();
  auto& unmatched_branch_actions = parser.rules()[0].back().unmatchedBranchActions();
  ASSERT_EQ(all_actions.size(), 3);
  ASSERT_EQ(matched_branch_actions.size(), 2);
  ASSERT_EQ(unmatched_branch_actions.size(), 2);

  const Action::SetEnv* all_action0 = dynamic_cast<const Action::SetEnv*>(all_actions[0].get());
  const Action::SetEnv* all_action1 = dynamic_cast<const Action::SetEnv*>(all_actions[1].get());
  const Action::SetEnv* all_action2 = dynamic_cast<const Action::SetEnv*>(all_actions[2].get());
  const Action::SetEnv* matched_action0 =
      dynamic_cast<const Action::SetEnv*>(matched_branch_actions[0]);
  const Action::SetEnv* matched_action1 =
      dynamic_cast<const Action::SetEnv*>(matched_branch_actions[1]);
  const Action::SetEnv* unmatched_action0 =
      dynamic_cast<const Action::SetEnv*>(unmatched_branch_actions[0]);
  const Action::SetEnv* unmatched_action1 =
      dynamic_cast<const Action::SetEnv*>(unmatched_branch_actions[1]);
  ASSERT_NE(all_action0, nullptr);
  ASSERT_NE(all_action1, nullptr);
  ASSERT_NE(all_action2, nullptr);
  ASSERT_NE(matched_action0, nullptr);
  ASSERT_NE(matched_action1, nullptr);
  ASSERT_NE(unmatched_action0, nullptr);
  ASSERT_NE(unmatched_action1, nullptr);

  EXPECT_EQ(all_action0, matched_action0);
  EXPECT_EQ(all_action1, unmatched_action0);
  EXPECT_EQ(all_action2, matched_action1);
  EXPECT_EQ(all_action2, unmatched_action1);

  EXPECT_EQ(std::string_view(all_actions[0]->name()), "setenv");
  EXPECT_EQ(std::string_view(all_actions[1]->name()), "setenv");
  EXPECT_EQ(std::string_view(all_actions[2]->name()), "setenv");
  EXPECT_EQ(all_action0->key(), "score0");
  EXPECT_EQ(all_action1->key(), "score1");
  EXPECT_EQ(all_action2->key(), "score2");
}

TEST_F(RuleActionParseTest, ActionSetRsc) {
  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,setrsc:'this is rsc0',!setrsc:'this is rsc1',*setrsc:'this is rsc2',msg:'aaa'")";

    Antlr4::Parser parser;
    auto result = parser.load(rule_directive);
    ASSERT_TRUE(result.has_value());

    auto& all_actions = parser.rules()[0].back().actions();
    auto& matched_branch_actions = parser.rules()[0].back().matchedBranchActions();
    auto& unmatched_branch_actions = parser.rules()[0].back().unmatchedBranchActions();
    ASSERT_EQ(all_actions.size(), 3);
    ASSERT_EQ(matched_branch_actions.size(), 2);
    ASSERT_EQ(unmatched_branch_actions.size(), 2);

    const Action::SetRsc* all_action0 = dynamic_cast<const Action::SetRsc*>(all_actions[0].get());
    const Action::SetRsc* all_action1 = dynamic_cast<const Action::SetRsc*>(all_actions[1].get());
    const Action::SetRsc* all_action2 = dynamic_cast<const Action::SetRsc*>(all_actions[2].get());
    const Action::SetRsc* matched_action0 =
        dynamic_cast<const Action::SetRsc*>(matched_branch_actions[0]);
    const Action::SetRsc* matched_action1 =
        dynamic_cast<const Action::SetRsc*>(matched_branch_actions[1]);
    const Action::SetRsc* unmatched_action0 =
        dynamic_cast<const Action::SetRsc*>(unmatched_branch_actions[0]);
    const Action::SetRsc* unmatched_action1 =
        dynamic_cast<const Action::SetRsc*>(unmatched_branch_actions[1]);
    ASSERT_NE(all_action0, nullptr);
    ASSERT_NE(all_action1, nullptr);
    ASSERT_NE(all_action2, nullptr);
    ASSERT_NE(matched_action0, nullptr);
    ASSERT_NE(matched_action1, nullptr);
    ASSERT_NE(unmatched_action0, nullptr);
    ASSERT_NE(unmatched_action1, nullptr);

    EXPECT_EQ(all_action0, matched_action0);
    EXPECT_EQ(all_action1, unmatched_action0);
    EXPECT_EQ(all_action2, matched_action1);
    EXPECT_EQ(all_action2, unmatched_action1);

    EXPECT_EQ(std::string_view(all_actions[0]->name()), "setrsc");
    EXPECT_EQ(std::string_view(all_actions[1]->name()), "setrsc");
    EXPECT_EQ(std::string_view(all_actions[2]->name()), "setrsc");
    EXPECT_EQ(all_action0->value(), "this is rsc0");
    EXPECT_EQ(all_action1->value(), "this is rsc1");
    EXPECT_EQ(all_action2->value(), "this is rsc2");
  }

  // Macro expansion
  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,setrsc:%{tx.0},msg:'aaa'")";

    Antlr4::Parser parser;
    auto result = parser.load(rule_directive);
    ASSERT_TRUE(result.has_value());

    auto& actions = parser.rules()[0].back().actions();
    EXPECT_EQ(actions.size(), 1);
    EXPECT_EQ(std::string_view(actions.back()->name()), "setrsc");
  }
}

TEST_F(RuleActionParseTest, ActionSetSid) {
  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,setsid:'this is sid0',!setsid:'this is sid1',*setsid:'this is sid2',msg:'aaa'")";
    Antlr4::Parser parser;
    auto result = parser.load(rule_directive);
    ASSERT_TRUE(result.has_value());

    auto& all_actions = parser.rules()[0].back().actions();
    auto& matched_branch_actions = parser.rules()[0].back().matchedBranchActions();
    auto& unmatched_branch_actions = parser.rules()[0].back().unmatchedBranchActions();
    ASSERT_EQ(all_actions.size(), 3);
    ASSERT_EQ(matched_branch_actions.size(), 2);
    ASSERT_EQ(unmatched_branch_actions.size(), 2);

    const Action::SetSid* all_action0 = dynamic_cast<const Action::SetSid*>(all_actions[0].get());
    const Action::SetSid* all_action1 = dynamic_cast<const Action::SetSid*>(all_actions[1].get());
    const Action::SetSid* all_action2 = dynamic_cast<const Action::SetSid*>(all_actions[2].get());
    const Action::SetSid* matched_action0 =
        dynamic_cast<const Action::SetSid*>(matched_branch_actions[0]);
    const Action::SetSid* matched_action1 =
        dynamic_cast<const Action::SetSid*>(matched_branch_actions[1]);
    const Action::SetSid* unmatched_action0 =
        dynamic_cast<const Action::SetSid*>(unmatched_branch_actions[0]);
    const Action::SetSid* unmatched_action1 =
        dynamic_cast<const Action::SetSid*>(unmatched_branch_actions[1]);
    ASSERT_NE(all_action0, nullptr);
    ASSERT_NE(all_action1, nullptr);
    ASSERT_NE(all_action2, nullptr);
    ASSERT_NE(matched_action0, nullptr);
    ASSERT_NE(matched_action1, nullptr);
    ASSERT_NE(unmatched_action0, nullptr);
    ASSERT_NE(unmatched_action1, nullptr);

    EXPECT_EQ(all_action0, matched_action0);
    EXPECT_EQ(all_action1, unmatched_action0);
    EXPECT_EQ(all_action2, matched_action1);
    EXPECT_EQ(all_action2, unmatched_action1);

    EXPECT_EQ(std::string_view(all_actions[0]->name()), "setsid");
    EXPECT_EQ(std::string_view(all_actions[1]->name()), "setsid");
    EXPECT_EQ(std::string_view(all_actions[2]->name()), "setsid");
    EXPECT_EQ(all_action0->value(), "this is sid0");
    EXPECT_EQ(all_action1->value(), "this is sid1");
    EXPECT_EQ(all_action2->value(), "this is sid2");
  }

  // Macro expansion
  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,setsid:%{tx.0},msg:'aaa'")";
    Antlr4::Parser parser;
    auto result = parser.load(rule_directive);
    ASSERT_TRUE(result.has_value());
    auto& actions = parser.rules()[0].back().actions();
    EXPECT_EQ(actions.size(), 1);
    EXPECT_EQ(std::string_view(actions.back()->name()), "setsid");
  }
}

TEST_F(RuleActionParseTest, ActionSetUid) {
  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,setuid:'this is uid0',!setuid:'this is uid1',*setuid:'this is uid2',msg:'aaa'")";
    Antlr4::Parser parser;
    auto result = parser.load(rule_directive);
    ASSERT_TRUE(result.has_value());

    auto& all_actions = parser.rules()[0].back().actions();
    auto& matched_branch_actions = parser.rules()[0].back().matchedBranchActions();
    auto& unmatched_branch_actions = parser.rules()[0].back().unmatchedBranchActions();
    ASSERT_EQ(all_actions.size(), 3);
    ASSERT_EQ(matched_branch_actions.size(), 2);
    ASSERT_EQ(unmatched_branch_actions.size(), 2);

    const Action::SetUid* all_action0 = dynamic_cast<const Action::SetUid*>(all_actions[0].get());
    const Action::SetUid* all_action1 = dynamic_cast<const Action::SetUid*>(all_actions[1].get());
    const Action::SetUid* all_action2 = dynamic_cast<const Action::SetUid*>(all_actions[2].get());
    const Action::SetUid* matched_action0 =
        dynamic_cast<const Action::SetUid*>(matched_branch_actions[0]);
    const Action::SetUid* matched_action1 =
        dynamic_cast<const Action::SetUid*>(matched_branch_actions[1]);
    const Action::SetUid* unmatched_action0 =
        dynamic_cast<const Action::SetUid*>(unmatched_branch_actions[0]);
    const Action::SetUid* unmatched_action1 =
        dynamic_cast<const Action::SetUid*>(unmatched_branch_actions[1]);
    ASSERT_NE(all_action0, nullptr);
    ASSERT_NE(all_action1, nullptr);
    ASSERT_NE(all_action2, nullptr);
    ASSERT_NE(matched_action0, nullptr);
    ASSERT_NE(matched_action1, nullptr);
    ASSERT_NE(unmatched_action0, nullptr);
    ASSERT_NE(unmatched_action1, nullptr);

    EXPECT_EQ(all_action0, matched_action0);
    EXPECT_EQ(all_action1, unmatched_action0);
    EXPECT_EQ(all_action2, matched_action1);
    EXPECT_EQ(all_action2, unmatched_action1);

    EXPECT_EQ(std::string_view(all_actions[0]->name()), "setuid");
    EXPECT_EQ(std::string_view(all_actions[1]->name()), "setuid");
    EXPECT_EQ(std::string_view(all_actions[2]->name()), "setuid");
    EXPECT_EQ(all_action0->value(), "this is uid0");
    EXPECT_EQ(all_action1->value(), "this is uid1");
    EXPECT_EQ(all_action2->value(), "this is uid2");
  }

  // Macro expansion
  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,setuid:%{tx.0},msg:'aaa'")";
    Antlr4::Parser parser;
    auto result = parser.load(rule_directive);
    ASSERT_TRUE(result.has_value());
    auto& actions = parser.rules()[0].back().actions();
    EXPECT_EQ(actions.size(), 1);
    EXPECT_EQ(std::string_view(actions.back()->name()), "setuid");
  }
}

TEST_F(RuleActionParseTest, ActionTransformation) {
  const std::string rule_directive =
      R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,auditlog,t:none,t:hexDecode,msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());
  auto& transforms = parser.rules()[0].back().transforms();
  EXPECT_TRUE(parser.rules()[0].back().isIgnoreDefaultTransform());
  EXPECT_NE(nullptr, dynamic_cast<Transformation::HexDecode*>(transforms[0].get()));

  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,auditlog,t:none,t:hexDecode123,msg:'aaa'")";
    Antlr4::Parser parser;
    auto result = parser.load(rule_directive);
    ASSERT_TRUE(!result.has_value());
  }
}

TEST_F(RuleActionParseTest, ActionAuditLog) {
  const std::string rule_directive =
      R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,auditlog,msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());
  EXPECT_TRUE(parser.rules()[0].back().auditLog());
}

TEST_F(RuleActionParseTest, ActionLog) {
  const std::string rule_directive =
      R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,log,msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());
  EXPECT_TRUE(parser.rules()[0].back().log());
}

TEST_F(RuleActionParseTest, ActionNoAuditLog) {
  const std::string rule_directive =
      R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,noauditlog,msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());
  ASSERT_TRUE(parser.rules()[0].back().noAuditLog());
}

TEST_F(RuleActionParseTest, ActionNoLog) {
  const std::string rule_directive =
      R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,nolog,msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());
  ASSERT_TRUE(parser.rules()[0].back().noLog());
}

TEST_F(RuleActionParseTest, ActionCapture) {
  const std::string rule_directive =
      R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,capture,msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());
  EXPECT_TRUE(parser.rules()[0].back().capture());
}

TEST_F(RuleActionParseTest, ActionMultiMatch) {
  const std::string rule_directive =
      R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,multiMatch,msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());
  EXPECT_TRUE(parser.rules()[0].back().multiMatch());
}

TEST_F(RuleActionParseTest, ActionAllow) {
  const std::string rule_directive =
      R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,allow,msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(parser.rules()[0].back().disruptive(), Rule::Disruptive::ALLOW);
}

TEST_F(RuleActionParseTest, ActionBlock) {
  const std::string rule_directive =
      R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,block,msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(parser.rules()[0].back().disruptive(), Rule::Disruptive::BLOCK);
}

TEST_F(RuleActionParseTest, ActionDeny) {
  const std::string rule_directive =
      R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,deny,msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(parser.rules()[0].back().disruptive(), Rule::Disruptive::DENY);
}

TEST_F(RuleActionParseTest, ActionDrop) {
  const std::string rule_directive =
      R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,drop,msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(parser.rules()[0].back().disruptive(), Rule::Disruptive::DROP);
}

TEST_F(RuleActionParseTest, ActionPass) {
  const std::string rule_directive =
      R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,pass,msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(parser.rules()[0].back().disruptive(), Rule::Disruptive::PASS);
}

TEST_F(RuleActionParseTest, ActionRedirect) {
  const std::string rule_directive =
      R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,redirect:http://www.srhino.com,msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(parser.rules()[0].back().disruptive(), Rule::Disruptive::REDIRECT);
  EXPECT_EQ(parser.rules()[0].back().redirect(), "http://www.srhino.com");
}

TEST_F(RuleActionParseTest, ActionStatus) {
  const std::string rule_directive =
      R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,status:500,msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(parser.rules()[0].back().status(), "500");
}

TEST_F(RuleActionParseTest, ActionXmlns) {
  const std::string rule_directive =
      R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,xmlns:xsd=http://www.w3.org/2001/XMLSchema,msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(parser.rules()[0].back().xmlns(), "xsd=http://www.w3.org/2001/XMLSchema");
}

TEST_F(RuleActionParseTest, ActionCtlAuditEngine) {
  const std::string rule_directive =
      R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,ctl:auditEngine=On,!ctl:auditEngine=On,*ctl:auditEngine=On,msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());

  auto& all_actions = parser.rules()[0].back().actions();
  auto& matched_branch_actions = parser.rules()[0].back().matchedBranchActions();
  auto& unmatched_branch_actions = parser.rules()[0].back().unmatchedBranchActions();
  ASSERT_EQ(all_actions.size(), 3);
  ASSERT_EQ(matched_branch_actions.size(), 2);
  ASSERT_EQ(unmatched_branch_actions.size(), 2);

  const Action::Ctl* all_action0 = dynamic_cast<const Action::Ctl*>(all_actions[0].get());
  const Action::Ctl* all_action1 = dynamic_cast<const Action::Ctl*>(all_actions[1].get());
  const Action::Ctl* all_action2 = dynamic_cast<const Action::Ctl*>(all_actions[2].get());
  const Action::Ctl* matched_action0 = dynamic_cast<const Action::Ctl*>(matched_branch_actions[0]);
  const Action::Ctl* matched_action1 = dynamic_cast<const Action::Ctl*>(matched_branch_actions[1]);
  const Action::Ctl* unmatched_action0 =
      dynamic_cast<const Action::Ctl*>(unmatched_branch_actions[0]);
  const Action::Ctl* unmatched_action1 =
      dynamic_cast<const Action::Ctl*>(unmatched_branch_actions[1]);
  ASSERT_NE(all_action0, nullptr);
  ASSERT_NE(all_action1, nullptr);
  ASSERT_NE(all_action2, nullptr);
  ASSERT_NE(matched_action0, nullptr);
  ASSERT_NE(matched_action1, nullptr);
  ASSERT_NE(unmatched_action0, nullptr);
  ASSERT_NE(unmatched_action1, nullptr);

  EXPECT_EQ(all_action0, matched_action0);
  EXPECT_EQ(all_action1, unmatched_action0);
  EXPECT_EQ(all_action2, matched_action1);
  EXPECT_EQ(all_action2, unmatched_action1);

  EXPECT_EQ(std::string_view(all_actions[0]->name()), "ctl");
  EXPECT_EQ(std::string_view(all_actions[1]->name()), "ctl");
  EXPECT_EQ(std::string_view(all_actions[2]->name()), "ctl");

  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,ctl:auditEngine=Off,msg:'aaa'")";
    Antlr4::Parser parser;
    auto result = parser.load(rule_directive);
    ASSERT_TRUE(result.has_value());
  }

  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,ctl:auditEngine=RelevantOnly,msg:'aaa'")";
    Antlr4::Parser parser;
    auto result = parser.load(rule_directive);
    ASSERT_TRUE(result.has_value());
  }

  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,ctl:auditEngine=asdfasdf,msg:'aaa'")";
    Antlr4::Parser parser;
    auto result = parser.load(rule_directive);
    ASSERT_TRUE(!result.has_value());
  }
}

TEST_F(RuleActionParseTest, ActionCtlAuditLogParts) {
  const std::string rule_directive =
      R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,ctl:auditLogParts=+ABCDEF,msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());

  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,ctl:auditLogParts=-ABCDEF,msg:'aaa'")";
    Antlr4::Parser parser;
    auto result = parser.load(rule_directive);
    ASSERT_TRUE(result.has_value());
  }

  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,ctl:auditLogParts=+ABCDEFL,msg:'aaa'")";
    Antlr4::Parser parser;
    auto result = parser.load(rule_directive);
    ASSERT_FALSE(result.has_value());
  }
}

TEST_F(RuleActionParseTest, ActionCtlRequestBodyAccess) {
  const std::string rule_directive =
      R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,ctl:requestBodyAccess=On,msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());

  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,ctl:requestBodyAccess=Off,msg:'aaa'")";
    Antlr4::Parser parser;
    auto result = parser.load(rule_directive);
    ASSERT_TRUE(result.has_value());
  }

  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,ctl:requestBodyAccess=Hi,msg:'aaa'")";
    Antlr4::Parser parser;
    auto result = parser.load(rule_directive);
    ASSERT_FALSE(result.has_value());
  }
}

TEST_F(RuleActionParseTest, ActionCtlRequestBodyProcessor) {
  const std::string rule_directive =
      R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,ctl:requestBodyProcessor=XML,msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());

  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,ctl:requestBodyProcessor=JSON,msg:'aaa'")";
    Antlr4::Parser parser;
    auto result = parser.load(rule_directive);
    ASSERT_TRUE(result.has_value());
  }

  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,ctl:requestBodyProcessor=Hi,msg:'aaa'")";
    Antlr4::Parser parser;
    auto result = parser.load(rule_directive);
    ASSERT_FALSE(result.has_value());
  }
}

TEST_F(RuleActionParseTest, ActionCtlRuleEngine) {
  const std::string rule_directive =
      R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,ctl:ruleEngine=On,msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());

  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,ctl:ruleEngine=Off,msg:'aaa'")";
    Antlr4::Parser parser;
    auto result = parser.load(rule_directive);
    ASSERT_TRUE(result.has_value());
  }

  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,ctl:ruleEngine=DetectionOnly,msg:'aaa'")";
    Antlr4::Parser parser;
    auto result = parser.load(rule_directive);
    ASSERT_TRUE(result.has_value());
  }

  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,ctl:ruleEngine=Hi,msg:'aaa'")";
    Antlr4::Parser parser;
    auto result = parser.load(rule_directive);
    ASSERT_FALSE(result.has_value());
  }
}

TEST_F(RuleActionParseTest, ActionCtlRuleRemoveById) {
  const std::string rule_directive =
      R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,ctl:ruleRemoveById=123,msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());

  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,ctl:ruleRemoveById=222-333,msg:'aaa'")";
    Antlr4::Parser parser;
    auto result = parser.load(rule_directive);
    ASSERT_TRUE(result.has_value());
  }
}

TEST_F(RuleActionParseTest, ActionCtlRuleRemoveByTag) {
  const std::string rule_directive =
      R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,ctl:ruleRemoveByTag=foo,msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());
}

TEST_F(RuleActionParseTest, ActionCtlRuleRemoveTargetById) {
  const std::string rule_directive =
      R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,ctl:ruleRemoveTargetById=123;ARGS:foo|ARGS:bar,msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());
}

TEST_F(RuleActionParseTest, ActionCtlRuleRemoveTargetByTag) {
  const std::string rule_directive =
      R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,ctl:ruleRemoveTargetByTag=foo;ARGS:foo|ARGS:bar,msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());
}

TEST_F(RuleActionParseTest, ActionChain) {
  const std::string rule_directive =
      R"(SecRule ARGS:aaa|ARGS:bbb "foo" "id:1,phase:1,ctl:ruleRemoveTargetByTag=foo;ARGS:foo|ARGS:bar,msg:'aaa',chain"
SecRule ARGS_GET|ARGS_POST:foo|!ARGS_GET:foo|&ARGS "bar" "id:2,tag:'foo',msg:'bar'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(parser.rules()[0].size(), 1);

  // Variables pool
  Rule* chain_rule = parser.rules()[0].back().chainRule(0);
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

  // Variables map
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

  // Operator
  auto& rule_operator = chain_rule->operators().front();
  EXPECT_EQ(rule_operator->name(), std::string("rx"));
  EXPECT_EQ(rule_operator->literalValue(), "bar");

  // Parent rule
  EXPECT_EQ(chain_rule->parentRule(), &parser.rules()[0].front());

  // Test when adding rule(vector expansion), the parentRule of chain rule is still correct.
  EXPECT_EQ(parser.rules()[0].capacity(), 1);
  const std::string rule_directive2 = R"(SecRule ARGS:aaa "foo" "id:2,phase:1,msg:'aaa'")";
  result = parser.load(rule_directive2);
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(parser.rules()[0].size(), 2);
  EXPECT_EQ(parser.rules()[0].capacity(), 2);
  EXPECT_EQ(chain_rule->parentRule(), &parser.rules()[0].front());
  const std::string rule_directive3 = R"(SecAction "phase:1,setvar:'tx.score=100'")";
  result = parser.load(rule_directive3);
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(parser.rules()[0].size(), 3);
  EXPECT_EQ(parser.rules()[0].capacity(), 4);
  EXPECT_EQ(chain_rule->parentRule(), &parser.rules()[0].front());
}

TEST_F(RuleActionParseTest, ActionInitCol) {
  const std::string rule_directive =
      R"(SecRule ARGS:aaa|ARGS:bbb "foo" "id:1,phase:1,initcol:global=global,initcol:ip=%{remote_addr}_%{MATCHED_VAR}")";

  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(parser.rules()[0].size(), 1);
  auto& actions = parser.rules()[0].back().actions();
  EXPECT_EQ(actions.size(), 2);
  EXPECT_NE(nullptr, dynamic_cast<Action::InitCol*>(actions.front().get()));

  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "foo" "id:1,phase:1,initcol:ip=%{remote_addr}_%{MATCHED_VAR},!initcol:ip=%{remote_addr}_%{MATCHED_VAR},*initcol:ip=%{remote_addr}_%{MATCHED_VAR}")";

    Antlr4::Parser parser;
    auto result = parser.load(rule_directive);
    ASSERT_TRUE(result.has_value());

    auto& all_actions = parser.rules()[0].back().actions();
    auto& matched_branch_actions = parser.rules()[0].back().matchedBranchActions();
    auto& unmatched_branch_actions = parser.rules()[0].back().unmatchedBranchActions();
    ASSERT_EQ(all_actions.size(), 3);
    ASSERT_EQ(matched_branch_actions.size(), 2);
    ASSERT_EQ(unmatched_branch_actions.size(), 2);

    const Action::InitCol* all_action0 = dynamic_cast<const Action::InitCol*>(all_actions[0].get());
    const Action::InitCol* all_action1 = dynamic_cast<const Action::InitCol*>(all_actions[1].get());
    const Action::InitCol* all_action2 = dynamic_cast<const Action::InitCol*>(all_actions[2].get());
    const Action::InitCol* matched_action0 =
        dynamic_cast<const Action::InitCol*>(matched_branch_actions[0]);
    const Action::InitCol* matched_action1 =
        dynamic_cast<const Action::InitCol*>(matched_branch_actions[1]);
    const Action::InitCol* unmatched_action0 =
        dynamic_cast<const Action::InitCol*>(unmatched_branch_actions[0]);
    const Action::InitCol* unmatched_action1 =
        dynamic_cast<const Action::InitCol*>(unmatched_branch_actions[1]);
    ASSERT_NE(all_action0, nullptr);
    ASSERT_NE(all_action1, nullptr);
    ASSERT_NE(all_action2, nullptr);
    ASSERT_NE(matched_action0, nullptr);
    ASSERT_NE(matched_action1, nullptr);
    ASSERT_NE(unmatched_action0, nullptr);
    ASSERT_NE(unmatched_action1, nullptr);

    EXPECT_EQ(all_action0, matched_action0);
    EXPECT_EQ(all_action1, unmatched_action0);
    EXPECT_EQ(all_action2, matched_action1);
    EXPECT_EQ(all_action2, unmatched_action1);

    EXPECT_EQ(std::string_view(all_actions[0]->name()), "initcol");
    EXPECT_EQ(std::string_view(all_actions[1]->name()), "initcol");
    EXPECT_EQ(std::string_view(all_actions[2]->name()), "initcol");
  }
}

TEST_F(RuleActionParseTest, ActionSkipAfter) {
  const std::string rule_directive =
      R"(SecRule ARGS:aaa|ARGS:bbb "foo" "id:1,phase:1,skipAfter:hi,msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(parser.rules()[0].back().skipAfter(), "hi");
}

TEST_F(RuleActionParseTest, ActionSkip) {
  const std::string rule_directive =
      R"(SecRule ARGS:aaa|ARGS:bbb "foo" "id:1,phase:1,skip:3,msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(parser.rules()[0].back().skip(), 3);
}

TEST_F(RuleActionParseTest, ActionServerity) {
  const std::string rule_directive =
      R"(SecRule ARGS:aaa|ARGS:bbb "foo" "id:1,phase:1,severity:2,msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(static_cast<uint32_t>(parser.rules()[0].back().severity()), 2);

  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "foo" "id:1,phase:1,severity:8,msg:'aaa'")";
    Antlr4::Parser parser;
    auto result = parser.load(rule_directive);
    ASSERT_FALSE(result.has_value());
  }

  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "foo" "id:1,phase:1,severity:'EMERGENCY',msg:'aaa'")";
    Antlr4::Parser parser;
    auto result = parser.load(rule_directive);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(static_cast<uint32_t>(parser.rules()[0].back().severity()), 0);
  }

  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "foo" "id:1,phase:1,severity:'ALERT',msg:'aaa'")";
    Antlr4::Parser parser;
    auto result = parser.load(rule_directive);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(static_cast<uint32_t>(parser.rules()[0].back().severity()), 1);
  }

  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "foo" "id:1,phase:1,severity:'CRITICAL',msg:'aaa'")";
    Antlr4::Parser parser;
    auto result = parser.load(rule_directive);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(static_cast<uint32_t>(parser.rules()[0].back().severity()), 2);
  }

  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "foo" "id:1,phase:1,severity:'ERROR',msg:'aaa'")";
    Antlr4::Parser parser;
    auto result = parser.load(rule_directive);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(static_cast<uint32_t>(parser.rules()[0].back().severity()), 3);
  }

  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "foo" "id:1,phase:1,severity:'WARNING',msg:'aaa'")";
    Antlr4::Parser parser;
    auto result = parser.load(rule_directive);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(static_cast<uint32_t>(parser.rules()[0].back().severity()), 4);
  }

  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "foo" "id:1,phase:1,severity:'NOTICE',msg:'aaa'")";
    Antlr4::Parser parser;
    auto result = parser.load(rule_directive);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(static_cast<uint32_t>(parser.rules()[0].back().severity()), 5);
  }

  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "foo" "id:1,phase:1,severity:'INFO',msg:'aaa'")";
    Antlr4::Parser parser;
    auto result = parser.load(rule_directive);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(static_cast<uint32_t>(parser.rules()[0].back().severity()), 6);
  }

  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "foo" "id:1,phase:1,severity:'DEBUG',msg:'aaa'")";
    Antlr4::Parser parser;
    auto result = parser.load(rule_directive);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(static_cast<uint32_t>(parser.rules()[0].back().severity()), 7);
  }

  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "foo" "id:1,phase:1,severity:'HI',msg:'aaa'")";
    Antlr4::Parser parser;
    auto result = parser.load(rule_directive);
    ASSERT_FALSE(result.has_value());
  }
}

TEST_F(RuleActionParseTest, ActionIdWithString) {
  {
    const std::string rule_directive =
        R"(SecRule ARGS_GET|ARGS_POST:foo|!ARGS_GET:foo|&ARGS "bar" "id:'123abc',phase:1,tag:'foo',msg:'bar'")";
    Antlr4::Parser parser;
    auto result = parser.load(rule_directive);

    // id must be a number
    ASSERT_FALSE(result.has_value());
  }

  const std::string rule_directive =
      R"(SecRule ARGS_GET|ARGS_POST:foo|!ARGS_GET:foo|&ARGS "bar" "id:'1',phase:1,tag:'foo',msg:'bar'")";

  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());

  // Variables pool
  EXPECT_EQ(parser.rules()[0].size(), 1);
  auto& rule_var_pool = parser.rules()[0].back().variables();
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

  auto& except_var_pool = parser.rules()[0].back().exceptVariables();
  ASSERT_EQ(except_var_pool.size(), 1);
  EXPECT_NE(nullptr, dynamic_cast<Variable::ArgsGet*>(except_var_pool[0].get()));
  EXPECT_EQ(except_var_pool[0]->subName(), "foo");
  EXPECT_FALSE(except_var_pool[0]->isCounter());
  EXPECT_TRUE(except_var_pool[0]->isNot());

  // variables map
  auto& rule_var_index = parser.rules()[0].back().variablesIndex();
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
  auto& rule_operator = parser.rules()[0].back().operators().front();
  EXPECT_EQ(rule_operator->name(), std::string("rx"));
  EXPECT_EQ(rule_operator->literalValue(), "bar");
}

TEST_F(RuleActionParseTest, ActionMsgWithMacro) {
  const std::string rule_directive =
      R"(SecRule ARGS_GET|ARGS_POST:foo|!ARGS_GET:foo|&ARGS "bar" "id:'111',phase:1,tag:'foo',msg:'foo: %{tx.foo} bar: %{tx.bar}'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);

  ASSERT_TRUE(result.has_value());

  EXPECT_TRUE(parser.rules()[0].back().msg().empty());
}

TEST_F(RuleActionParseTest, ActionLogData) {
  const std::string rule_directive =
      R"(SecRule ARGS:aaa|ARGS:bbb "foo" "id:1,phase:1,logdata:'this is logdata',msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(parser.rules()[0].back().logdata(), "this is logdata");
}

TEST_F(RuleActionParseTest, ActionLogDataWithMacro) {
  const std::string rule_directive =
      R"(SecRule ARGS:aaa|ARGS:bbb "foo" "id:1,phase:1,logdata:'foo: %{tx.foo} bar: %{tx.bar}',msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());

  EXPECT_TRUE(parser.rules()[0].back().logdata().empty());
}

TEST_F(RuleActionParseTest, ActionFirstMatch) {
  const std::string rule_directive = R"(SecRule ARGS "foo" "id:1,phase:1,firstMatch,msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());

  EXPECT_TRUE(parser.rules()[0].back().firstMatch());
}

TEST_F(RuleActionParseTest, ActionEmptyMatch) {
  const std::string rule_directive =
      R"(SecRule ARGS "@rx %{tx.foo}" "id:1,phase:1,emptyMatch,msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());

  EXPECT_TRUE(parser.rules()[0].back().emptyMatch());
}

TEST_F(RuleActionParseTest, ActionMultiChain) {
  {
    const std::string rule_directive = R"(
        SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,multiChain, msg:'aaa'"
          SecRule ARGS:ccc "baz" "id:2,phase:1,msg:'bbb'")";

    Antlr4::Parser parser;
    auto result = parser.load(rule_directive);
    ASSERT_TRUE(result.has_value());

    auto& rule = parser.rules()[0].back();

    EXPECT_TRUE(rule.matchedMultiChain());
    EXPECT_FALSE(rule.unmatchedMultiChain());
  }

  {
    const std::string rule_directive = R"(
        SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,!multiChain, msg:'aaa'"
          SecRule ARGS:ccc "baz" "id:2,phase:1,msg:'bbb'")";

    Antlr4::Parser parser;
    auto result = parser.load(rule_directive);
    ASSERT_TRUE(result.has_value());

    auto& rule = parser.rules()[0].back();

    EXPECT_FALSE(rule.matchedMultiChain());
    EXPECT_TRUE(rule.unmatchedMultiChain());
  }

  {
    const std::string rule_directive = R"(
        SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,*multiChain, msg:'aaa'"
          SecRule ARGS:ccc "baz" "id:2,phase:1,msg:'bbb'")";

    Antlr4::Parser parser;
    auto result = parser.load(rule_directive);
    ASSERT_TRUE(result.has_value());

    auto& rule = parser.rules()[0].back();

    EXPECT_TRUE(rule.matchedMultiChain());
    EXPECT_TRUE(rule.unmatchedMultiChain());
  }
}

TEST_F(RuleActionParseTest, ActionAlias) {
  Antlr4::Parser parser;

  // 1. Test alias is defined correctly and stored in parser as lowercase.
  // 2. Test alias in variable list is parsed correctly.
  // 3. Test alias in macro is parsed correctly.
  // 4. Test the alias were cleared after parsing is done.
  const std::string rule_directive = R"(
        SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,alias:test0=MATCHED_OPTREE,alias:test1=MATCHED_VPTREE../../foo.bar,alias:test2=MATCHED_OPTREE../,msg:'aaa',chain"
          SecRule test0|test1:world "@rx %{test2.for.bar}" "id:2,phase:1,msg:'bbb'"
        SecRule test0|test1:world "baz" "id:3,phase:1,msg:'bbb'")";

  auto result = parser.load(rule_directive);
  ASSERT_FALSE(result.has_value());

  // The last rule should be parsed error since the alias were cleared after parsing is done.
  EXPECT_EQ(parser.rules()[0].size(), 1);

  auto& rule_var_pool = parser.rules()[0].front().chainRule(0)->variables();
  ASSERT_EQ(rule_var_pool.size(), 2);
  Variable::MatchedOPTree* var0 = dynamic_cast<Variable::MatchedOPTree*>(rule_var_pool[0].get());
  Variable::MatchedVPTree* var1 = dynamic_cast<Variable::MatchedVPTree*>(rule_var_pool[1].get());
  ASSERT_NE(var0, nullptr);
  ASSERT_NE(var1, nullptr);
  EXPECT_EQ(var0->subName(), "");
  EXPECT_EQ(var1->subName(), "../../foo.bar.world");
  EXPECT_EQ(var0->parentCount(), 0);
  EXPECT_EQ(var1->parentCount(), 2);
  EXPECT_EQ(var0->paths().size(), 0);
  EXPECT_EQ(var1->paths().size(), 3);

  auto& op = parser.rules()[0].front().chainRule(0)->operators().front();
  EXPECT_EQ(op->literalValue(), "");
  EXPECT_EQ(op->macro()->literalValue(), "%{MATCHED_OPTREE.for.bar}");
  Macro::VariableMacro* op_var_macro = dynamic_cast<Macro::VariableMacro*>(op->macro().get());
  ASSERT_NE(op_var_macro, nullptr);
  EXPECT_EQ(op_var_macro->getVariable()->subName(), "for.bar");
}
} // namespace Parser
} // namespace Wge