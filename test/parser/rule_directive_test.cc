#include <functional>
#include <string>
#include <vector>

#include <gtest/gtest.h>
#include <stdlib.h>

#include "antlr4/parser.h"
#include "engine.h"

namespace SrSecurity {
namespace Parser {
class RuleTest : public testing::Test {
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

  Engine engine_;
};

TEST_F(RuleTest, Rule) {
  const std::string rule_directive =
      R"(SecRule ARGS_GET|ARGS_POST:foo|!ARGS_GET:foo|&ARGS "bar" "id:1,tag:'foo',msg:'bar'")";
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

TEST_F(RuleTest, RuleUpdateActionById) {
  const std::string rule_directive = R"(SecRule ARGS "bar" "id:1,tag:'tag1',msg:'msg1'")";

  Antlr4::Parser parser;
  std::string error = parser.load(rule_directive);
  ASSERT_TRUE(error.empty());
  EXPECT_EQ(parser.rules().back()->msg(), "msg1");

  {
    const std::string rule_update = R"(SecRuleUpdateActionById 1 "msg:'msg2'")";
    error = parser.load(rule_update);
    ASSERT_TRUE(error.empty());
    EXPECT_EQ(parser.rules().back()->msg(), "msg2");
  }

  {
    auto& tags = parser.rules().back()->tags();
    EXPECT_NE(tags.find("tag1"), tags.end());
    EXPECT_EQ(tags.find("tag2"), tags.end());
    EXPECT_EQ(tags.find("tag3"), tags.end());

    const std::string rule_update =
        R"(SecRuleUpdateActionById 1 "msg:'msg3',tag:'tag2',tag:'tag3'")";
    error = parser.load(rule_update);
    ASSERT_TRUE(error.empty());
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

TEST_F(RuleTest, RuleUpdateTargetByMsg) {
  const std::string rule_directive =
      R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,tag:'tag1',msg:'msg1'")";

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

TEST_F(RuleTest, RuleUpdateTargetByTag) {
  const std::string rule_directive =
      R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,tag:'tag1',msg:'msg1'")";

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

TEST_F(RuleTest, ActionSetVar) {
  auto t = engine_.makeTransaction();

  // Create
  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,setvar:tx.score,msg:'aaa'")";
    Antlr4::Parser parser;
    std::string error = parser.load(rule_directive);
    ASSERT_TRUE(error.empty());
    auto& actions = parser.rules().back()->actions();
    EXPECT_EQ(actions.size(), 1);
    actions.back()->evaluate(*t);
    int score = t->getVariableInt("score");
    EXPECT_EQ(score, 1);
  }

  // Create and init
  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:2,setvar:tx.score2=100,msg:'aaa'")";
    Antlr4::Parser parser;
    std::string error = parser.load(rule_directive);
    ASSERT_TRUE(error.empty());
    auto& actions = parser.rules().back()->actions();
    EXPECT_EQ(actions.size(), 1);
    actions.back()->evaluate(*t);
    int score = t->getVariableInt("score2");
    EXPECT_EQ(score, 100);
  }

  // Create and init (Macro expansion)
  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:2,setvar:tx.score3=%{tx.score2},msg:'aaa'")";
    Antlr4::Parser parser;
    std::string error = parser.load(rule_directive);
    ASSERT_TRUE(error.empty());
    auto& actions = parser.rules().back()->actions();
    EXPECT_EQ(actions.size(), 1);
    actions.back()->evaluate(*t);
    int score2 = t->getVariableInt("score2");
    int score3 = t->getVariableInt("score3");
    EXPECT_EQ(score2, score3);
  }

  // Remove
  {
    EXPECT_NE(nullptr, t->getVariable("score2"));
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:3,setvar:!tx.score2,msg:'aaa'")";
    Antlr4::Parser parser;
    std::string error = parser.load(rule_directive);
    ASSERT_TRUE(error.empty());
    auto& actions = parser.rules().back()->actions();
    EXPECT_EQ(actions.size(), 1);
    actions.back()->evaluate(*t);
    EXPECT_EQ(nullptr, t->getVariable("score2"));
  }

  // Increase
  {
    int old_score = t->getVariableInt("score");
    EXPECT_NE(old_score, 0);
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:4,setvar:tx.score=+100,msg:'aaa'")";
    Antlr4::Parser parser;
    std::string error = parser.load(rule_directive);
    ASSERT_TRUE(error.empty());
    auto& actions = parser.rules().back()->actions();
    EXPECT_EQ(actions.size(), 1);
    actions.back()->evaluate(*t);
    int new_score = t->getVariableInt("score");
    EXPECT_EQ(new_score, old_score + 100);
  }

  // Increase (Macro expansion)
  {
    int old_score = t->getVariableInt("score");
    EXPECT_NE(old_score, 0);
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:4,setvar:tx.score=+%{tx.score},msg:'aaa'")";
    Antlr4::Parser parser;
    std::string error = parser.load(rule_directive);
    ASSERT_TRUE(error.empty());
    auto& actions = parser.rules().back()->actions();
    EXPECT_EQ(actions.size(), 1);
    actions.back()->evaluate(*t);
    int new_score = t->getVariableInt("score");
    EXPECT_EQ(old_score, new_score - old_score);
  }

  // Decrease
  {
    int old_score = t->getVariableInt("score");
    EXPECT_NE(old_score, 0);
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:5,setvar:tx.score=-50,msg:'aaa'")";
    Antlr4::Parser parser;
    std::string error = parser.load(rule_directive);
    ASSERT_TRUE(error.empty());
    auto& actions = parser.rules().back()->actions();
    EXPECT_EQ(actions.size(), 1);
    actions.back()->evaluate(*t);
    int new_score = t->getVariableInt("score");
    EXPECT_EQ(old_score, new_score + 50);
  }

  // Decrease (Macro expansion)
  {
    int old_score = t->getVariableInt("score");
    EXPECT_NE(old_score, 0);
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:5,setvar:tx.score=-%{tx.score},msg:'aaa'")";
    Antlr4::Parser parser;
    std::string error = parser.load(rule_directive);
    ASSERT_TRUE(error.empty());
    auto& actions = parser.rules().back()->actions();
    EXPECT_EQ(actions.size(), 1);
    actions.back()->evaluate(*t);
    int new_score = t->getVariableInt("score");
    EXPECT_EQ(old_score, new_score + old_score);
  }
}

TEST_F(RuleTest, ActionSetEnv) {
  auto t = engine_.makeTransaction();
  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,setenv:var1=hello,msg:'aaa'")";
    Antlr4::Parser parser;
    std::string error = parser.load(rule_directive);
    ASSERT_TRUE(error.empty());
    auto& actions = parser.rules().back()->actions();
    EXPECT_EQ(actions.size(), 1);
    actions.back()->evaluate(*t);
    EXPECT_EQ(std::string("hello"), ::getenv("var1"));
  }
}
} // namespace Parser
} // namespace SrSecurity