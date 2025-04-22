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

namespace Wge {
namespace Parser {
class RuleTest : public testing::Test {
private:
  // Use for specific the main thread id, so that the ASSERT_IS_MAIN_THREAD macro can work
  // correctly in the test.
  Engine main_thread_id_init_helper_;
};

TEST_F(RuleTest, Rule) {
  const std::string rule_directive =
      R"(SecRule ARGS_GET|ARGS_POST:foo|!ARGS_GET:foo|&ARGS "bar" "id:1,tag:'foo',msg:'bar'")";

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

TEST_F(RuleTest, RuleRemoveById) {
  const std::string rule_directive = R"(SecRule ARGS "bar" "id:1,tag:'tag1',msg:'msg1'"
  SecRule ARGS "bar" "id:2,tag:'tag2',tag:'tag3',msg:'msg2'"
  SecRule ARGS "bar" "id:3,tag:'tag2',tag:'tag3',msg:'msg3'"
  SecRule ARGS "bar" "id:4,tag:'tag4',msg:'msg4'"
  SecRule ARGS "bar" "id:5,tag:'tag5',msg:'msg5'"
  SecRule ARGS "bar" "id:6,tag:'tag6',msg:'msg6'"
  SecRule ARGS "bar" "id:7,tag:'tag6',msg:'msg6'"
  SecRule ARGS "bar" "id:8,tag:'tag6',msg:'msg6'"
  SecRule ARGS "bar" "id:9,tag:'tag6',msg:'msg6'"
  SecRule ARGS "bar" "id:10,tag:'tag6',msg:'msg6'"
  )";

  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());

  auto& rules = parser.rules();
  EXPECT_EQ(rules.size(), 10);

  {
    const std::string rule_remove = R"(SecRuleRemoveById 1)";
    auto result = parser.load(rule_remove);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(rules.size(), 9);
    auto iter = std::find_if(rules.begin(), rules.end(),
                             [](const std::unique_ptr<Rule>& rule) { return rule->id() == 1; });
    EXPECT_EQ(iter, rules.end());
  }

  {
    const std::string rule_remove = R"(SecRuleRemoveById 2 3)";
    auto result = parser.load(rule_remove);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(rules.size(), 7);
    auto iter = std::find_if(rules.begin(), rules.end(), [](const std::unique_ptr<Rule>& rule) {
      return rule->id() == 2 || rule->id() == 3;
    });
    EXPECT_EQ(iter, rules.end());
  }

  {
    const std::string rule_remove = R"(SecRuleRemoveById 4 5 6-8)";
    auto result = parser.load(rule_remove);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(rules.size(), 2);
    auto iter = std::find_if(rules.begin(), rules.end(), [](const std::unique_ptr<Rule>& rule) {
      return rule->id() == 4 || rule->id() == 5 || rule->id() == 6 || rule->id() == 7 ||
             rule->id() == 8;
    });
    EXPECT_EQ(iter, rules.end());
  }
}

TEST_F(RuleTest, RuleRemoveByMsg) {
  const std::string rule_directive = R"(SecRule ARGS "bar" "id:1,tag:'tag1',msg:'msg1'"
  SecRule ARGS "bar" "id:2,tag:'tag2',tag:'tag3',msg:'msg2'"
  SecRule ARGS "bar" "id:3,tag:'tag2',tag:'tag3',msg:'msg3'"
  SecRule ARGS "bar" "id:4,tag:'tag4',msg:'msg4'"
  SecRule ARGS "bar" "id:5,tag:'tag5',msg:'msg5'"
  SecRule ARGS "bar" "id:6,tag:'tag6',msg:'msg6'"
  SecRule ARGS "bar" "id:7,tag:'tag6',msg:'msg6'"
  SecRule ARGS "bar" "id:8,tag:'tag6',msg:'msg6'"
  SecRule ARGS "bar" "id:9,tag:'tag6',msg:'msg6'"
  SecRule ARGS "bar" "id:10,tag:'tag6',msg:'msg6'"
  )";

  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());

  auto& rules = parser.rules();
  EXPECT_EQ(rules.size(), 10);

  {
    const std::string rule_remove = R"(SecRuleRemoveByMsg "msg1")";
    auto result = parser.load(rule_remove);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(rules.size(), 9);
    auto iter = std::find_if(rules.begin(), rules.end(),
                             [](const std::unique_ptr<Rule>& rule) { return rule->id() == 1; });
    EXPECT_EQ(iter, rules.end());
  }

  {
    const std::string rule_remove = R"(SecRuleRemoveByMsg "msg6")";
    auto result = parser.load(rule_remove);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(rules.size(), 4);
    auto iter = std::find_if(rules.begin(), rules.end(), [](const std::unique_ptr<Rule>& rule) {
      return rule->id() == 6 || rule->id() == 7 || rule->id() == 8 || rule->id() == 9 ||
             rule->id() == 10;
    });
    EXPECT_EQ(iter, rules.end());
  }
}

TEST_F(RuleTest, RuleRemoveByTag) {
  const std::string rule_directive = R"(SecRule ARGS "bar" "id:1,tag:'tag1',msg:'msg1'"
  SecRule ARGS "bar" "id:2,tag:'tag2',tag:'tag3',msg:'msg2'"
  SecRule ARGS "bar" "id:3,tag:'tag2',tag:'tag3',msg:'msg3'"
  SecRule ARGS "bar" "id:4,tag:'tag4',msg:'msg4'"
  SecRule ARGS "bar" "id:5,tag:'tag5',msg:'msg5'"
  SecRule ARGS "bar" "id:6,tag:'tag6',msg:'msg6'"
  SecRule ARGS "bar" "id:7,tag:'tag6',msg:'msg6'"
  SecRule ARGS "bar" "id:8,tag:'tag6',msg:'msg6'"
  SecRule ARGS "bar" "id:9,tag:'tag6',msg:'msg6'"
  SecRule ARGS "bar" "id:10,tag:'tag6',msg:'msg6'"
  )";

  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());

  auto& rules = parser.rules();
  EXPECT_EQ(rules.size(), 10);

  {
    const std::string rule_remove = R"(SecRuleRemoveByTag "tag1")";
    auto result = parser.load(rule_remove);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(rules.size(), 9);
    auto iter = std::find_if(rules.begin(), rules.end(),
                             [](const std::unique_ptr<Rule>& rule) { return rule->id() == 1; });
    EXPECT_EQ(iter, rules.end());
  }

  {
    const std::string rule_remove = R"(SecRuleRemoveByTag "tag2")";
    auto result = parser.load(rule_remove);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(rules.size(), 7);
    auto iter = std::find_if(rules.begin(), rules.end(), [](const std::unique_ptr<Rule>& rule) {
      return rule->id() == 2 || rule->id() == 3;
    });
    EXPECT_EQ(iter, rules.end());
  }

  {
    const std::string rule_remove = R"(SecRuleRemoveByTag "tag6")";
    auto result = parser.load(rule_remove);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(rules.size(), 2);
    auto iter = std::find_if(rules.begin(), rules.end(), [](const std::unique_ptr<Rule>& rule) {
      return rule->id() == 6 || rule->id() == 7 || rule->id() == 8 || rule->id() == 9 ||
             rule->id() == 10;
    });
    EXPECT_EQ(iter, rules.end());
  }
}

TEST_F(RuleTest, RuleUpdateActionById) {
  const std::string rule_directive = R"(SecRule ARGS "bar" "id:1,tag:'tag1',msg:'msg1'")";

  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(parser.rules().back()->msg(), "msg1");

  {
    const std::string rule_update = R"(SecRuleUpdateActionById 1 "msg:'msg2'")";
    auto result = parser.load(rule_update);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(parser.rules().back()->msg(), "msg2");
  }

  {
    auto& tags = parser.rules().back()->tags();
    EXPECT_NE(tags.find("tag1"), tags.end());
    EXPECT_EQ(tags.find("tag2"), tags.end());
    EXPECT_EQ(tags.find("tag3"), tags.end());

    const std::string rule_update =
        R"(SecRuleUpdateActionById 1 "msg:'msg3',tag:'tag2',tag:'tag3'")";
    auto result = parser.load(rule_update);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(parser.rules().back()->msg(), "msg3");
    EXPECT_EQ(tags.find("tag1"), tags.end());
    EXPECT_NE(tags.find("tag2"), tags.end());
    EXPECT_NE(tags.find("tag3"), tags.end());
  }
}

TEST_F(RuleTest, RuleUpdateTargetById) {
  const std::string rule_directive =
      R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,tag:'tag1',msg:'msg1'")";

  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());
  auto& variable_index = parser.rules().back()->variablesIndex();
  EXPECT_NE(variable_index.find({"ARGS", "aaa"}), variable_index.end());
  EXPECT_NE(variable_index.find({"ARGS", "bbb"}), variable_index.end());
  EXPECT_FALSE(variable_index.find({"ARGS", "aaa"})->second.isNot());
  EXPECT_FALSE(variable_index.find({"ARGS", "bbb"})->second.isNot());

  {
    const std::string rule_update = R"(SecRuleUpdateTargetById 1 ARGS:ccc
SecRule ARGS:aaa|ARGS:bbb "bar" "id:2,tag:'tag1',msg:'msg1'")";
    auto result = parser.load(rule_update);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(parser.rules().size(), 2);
    EXPECT_NE(variable_index.find({"ARGS", "ccc"}), variable_index.end());
  }

  {
    const std::string rule_update = R"(SecRuleUpdateTargetById 1 !ARGS:aaa|!ARGS:bbb)";
    auto result = parser.load(rule_update);
    ASSERT_TRUE(result.has_value());
    EXPECT_NE(variable_index.find({"ARGS", "aaa"}), variable_index.end());
    EXPECT_NE(variable_index.find({"ARGS", "bbb"}), variable_index.end());
    EXPECT_NE(variable_index.find({"ARGS", "ccc"}), variable_index.end());

    auto& except_variables = parser.rules().front()->exceptVariables();
    EXPECT_EQ(except_variables.size(), 2);
    EXPECT_EQ(except_variables[0]->subName(), "aaa");
    EXPECT_EQ(except_variables[1]->subName(), "bbb");
    EXPECT_TRUE(except_variables[0]->isNot());
    EXPECT_TRUE(except_variables[1]->isNot());
  }
}

TEST_F(RuleTest, RuleUpdateTargetByMsg) {
  const std::string rule_directive =
      R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,tag:'tag1',msg:'msg1'")";

  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());
  auto& variable_index = parser.rules().back()->variablesIndex();
  EXPECT_NE(variable_index.find({"ARGS", "aaa"}), variable_index.end());
  EXPECT_NE(variable_index.find({"ARGS", "bbb"}), variable_index.end());
  EXPECT_FALSE(variable_index.find({"ARGS", "aaa"})->second.isNot());
  EXPECT_FALSE(variable_index.find({"ARGS", "bbb"})->second.isNot());

  {
    const std::string rule_update = R"(SecRuleUpdateTargetByMsg "msg1" ARGS:ccc)";
    auto result = parser.load(rule_update);
    ASSERT_TRUE(result.has_value());
    EXPECT_NE(variable_index.find({"ARGS", "ccc"}), variable_index.end());
  }

  {
    const std::string rule_update = R"(SecRuleUpdateTargetByMsg "msg1" !ARGS:aaa|!ARGS:bbb)";
    auto result = parser.load(rule_update);
    ASSERT_TRUE(result.has_value());
    EXPECT_NE(variable_index.find({"ARGS", "aaa"}), variable_index.end());
    EXPECT_NE(variable_index.find({"ARGS", "bbb"}), variable_index.end());
    EXPECT_NE(variable_index.find({"ARGS", "ccc"}), variable_index.end());

    auto& except_variables = parser.rules().back()->exceptVariables();
    EXPECT_EQ(except_variables.size(), 2);
    EXPECT_EQ(except_variables[0]->subName(), "aaa");
    EXPECT_EQ(except_variables[1]->subName(), "bbb");
    EXPECT_TRUE(except_variables[0]->isNot());
    EXPECT_TRUE(except_variables[1]->isNot());
  }
}

TEST_F(RuleTest, RuleUpdateTargetByTag) {
  const std::string rule_directive =
      R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,tag:'tag1',msg:'msg1'")";

  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());
  auto& variable_index = parser.rules().back()->variablesIndex();
  EXPECT_NE(variable_index.find({"ARGS", "aaa"}), variable_index.end());
  EXPECT_NE(variable_index.find({"ARGS", "bbb"}), variable_index.end());
  EXPECT_FALSE(variable_index.find({"ARGS", "aaa"})->second.isNot());
  EXPECT_FALSE(variable_index.find({"ARGS", "bbb"})->second.isNot());

  {
    const std::string rule_update = R"(SecRuleUpdateTargetByTag "tag1" ARGS:ccc)";
    auto result = parser.load(rule_update);
    ASSERT_TRUE(result.has_value());
    EXPECT_NE(variable_index.find({"ARGS", "ccc"}), variable_index.end());
  }

  {
    const std::string rule_update = R"(SecRuleUpdateTargetByTag "tag1" !ARGS:aaa|!ARGS:bbb)";
    auto result = parser.load(rule_update);
    ASSERT_TRUE(result.has_value());
    EXPECT_NE(variable_index.find({"ARGS", "aaa"}), variable_index.end());
    EXPECT_NE(variable_index.find({"ARGS", "bbb"}), variable_index.end());
    EXPECT_NE(variable_index.find({"ARGS", "ccc"}), variable_index.end());

    auto& except_variables = parser.rules().back()->exceptVariables();
    EXPECT_EQ(except_variables.size(), 2);
    EXPECT_EQ(except_variables[0]->subName(), "aaa");
    EXPECT_EQ(except_variables[1]->subName(), "bbb");
    EXPECT_TRUE(except_variables[0]->isNot());
    EXPECT_TRUE(except_variables[1]->isNot());
  }
}

TEST_F(RuleTest, Marker) {
  const std::string rule_directive =
      R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,tag:'tag1',msg:'msg1'"
SecMarker "Hi")";

  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());
  auto& rules = parser.rules();
  auto& markers = parser.markers();
  EXPECT_EQ(rules.size(), 1);
  EXPECT_EQ(markers.size(), 1);
}

TEST_F(RuleTest, NoAction) {
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
} // namespace Parser
} // namespace Wge