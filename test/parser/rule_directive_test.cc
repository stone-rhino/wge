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
class RuleTest : public testing::Test {
public:
  RuleTest() : engine_(spdlog::level::trace) {}

public:
  Engine engine_;
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
  ASSERT_EQ(rule_var_pool.size(), 4);
  EXPECT_NE(nullptr, dynamic_cast<Variable::ArgsGet*>(rule_var_pool[0].get()));
  EXPECT_EQ(rule_var_pool[0]->subName(), "");
  EXPECT_FALSE(rule_var_pool[0]->isCounter());
  EXPECT_FALSE(rule_var_pool[0]->isNot());

  EXPECT_NE(nullptr, dynamic_cast<Variable::ArgsPost*>(rule_var_pool[1].get()));
  EXPECT_EQ(rule_var_pool[1]->subName(), "foo");
  EXPECT_FALSE(rule_var_pool[1]->isCounter());
  EXPECT_FALSE(rule_var_pool[1]->isNot());

  EXPECT_NE(nullptr, dynamic_cast<Variable::ArgsGet*>(rule_var_pool[2].get()));
  EXPECT_EQ(rule_var_pool[2]->subName(), "foo");
  EXPECT_FALSE(rule_var_pool[2]->isCounter());
  EXPECT_TRUE(rule_var_pool[2]->isNot());

  EXPECT_NE(nullptr, dynamic_cast<Variable::Args*>(rule_var_pool[3].get()));
  EXPECT_EQ(rule_var_pool[3]->subName(), "");
  EXPECT_TRUE(rule_var_pool[3]->isCounter());
  EXPECT_FALSE(rule_var_pool[3]->isNot());

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

TEST_F(RuleTest, OperatorBeginWith) {
  const std::string rule_directive =
      R"(SecRule ARGS "@beginsWith hello" "id:1,tag:'foo',msg:'bar'")";

  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());

  auto& op = parser.rules().back()->getOperator();
  EXPECT_EQ(op->name(), std::string("beginsWith"));
  EXPECT_EQ(op->literalValue(), "hello");

  // Macro expansion
  {
    auto t = engine_.makeTransaction();
    const std::string rule_directive =
        R"(SecAction "setvar:tx.foo=bar"
        SecRule ARGS "@beginsWith %{tx.foo}" "id:1,tag:'foo',msg:'bar'")";

    Antlr4::Parser parser;
    auto result = parser.load(rule_directive);
    ASSERT_TRUE(result.has_value());

    parser.rules().front()->actions().front()->evaluate(*t);
    EXPECT_EQ(std::get<std::string>(t->getVariable("foo")), "bar");

    auto& op = parser.rules().back()->getOperator();
    EXPECT_EQ(op->name(), std::string("beginsWith"));
    EXPECT_TRUE(op->literalValue().empty());
    auto macro = op->macro();
    EXPECT_EQ(std::get<std::string>(macro->evaluate(*t)), "bar");
  }
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
    EXPECT_TRUE(variable_index.find({"ARGS", "aaa"})->second.isNot());
    EXPECT_TRUE(variable_index.find({"ARGS", "bbb"})->second.isNot());
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
    EXPECT_TRUE(variable_index.find({"ARGS", "aaa"})->second.isNot());
    EXPECT_TRUE(variable_index.find({"ARGS", "bbb"})->second.isNot());
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
    EXPECT_TRUE(variable_index.find({"ARGS", "aaa"})->second.isNot());
    EXPECT_TRUE(variable_index.find({"ARGS", "bbb"})->second.isNot());
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

TEST_F(RuleTest, ActionSetVar) {
  auto t = engine_.makeTransaction();

  // Create
  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,setvar:'tx.score',msg:'aaa'")";
    Antlr4::Parser parser;
    auto result = parser.load(rule_directive);
    ASSERT_TRUE(result.has_value());
    auto& actions = parser.rules().back()->actions();
    EXPECT_EQ(actions.size(), 1);
    actions.back()->evaluate(*t);
    int score = std::get<int>(t->getVariable("score"));
    EXPECT_EQ(score, 1);
  }

  // Create (Macro expansion)
  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:2,setvar:'tx.foo=bar',msg:'aaa'"
        SecRule ARGS:aaa|ARGS:bbb "bar" "id:3,setvar:'tx.%{tx.foo}score',msg:'aaa'")";
    Antlr4::Parser parser;
    auto result = parser.load(rule_directive);
    ASSERT_TRUE(result.has_value());
    auto& actions1 = parser.rules().front()->actions();
    auto& actions2 = parser.rules().back()->actions();
    EXPECT_EQ(actions1.size(), 1);
    EXPECT_EQ(actions2.size(), 1);
    actions1.back()->evaluate(*t);
    actions2.back()->evaluate(*t);
    EXPECT_EQ(std::get<std::string>(t->getVariable("foo")), "bar");
    int score = std::get<int>(t->getVariable("barscore"));
    EXPECT_EQ(score, 1);
  }

  // Create and init
  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:4,setvar:'tx.score2=100',msg:'aaa'")";
    Antlr4::Parser parser;
    auto result = parser.load(rule_directive);
    ASSERT_TRUE(result.has_value());
    auto& actions = parser.rules().back()->actions();
    EXPECT_EQ(actions.size(), 1);
    actions.back()->evaluate(*t);
    int score = std::get<int>(t->getVariable("score2"));
    EXPECT_EQ(score, 100);
  }

  // Create and init (Macro expansion)
  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:5,setvar:'tx.score_%{tx.foo}=%{tx.score2}',msg:'aaa'")";
    Antlr4::Parser parser;
    auto result = parser.load(rule_directive);
    ASSERT_TRUE(result.has_value());
    auto& actions = parser.rules().back()->actions();
    EXPECT_EQ(actions.size(), 1);
    actions.back()->evaluate(*t);
    int score2 = std::get<int>(t->getVariable("score2"));
    int score_bar = std::get<int>(t->getVariable("score_bar"));
    EXPECT_EQ(score2, score_bar);
  }

  // Create and init (Multi macro expansion)
  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:6,setvar:'tx.foo2=%{tx.score2}_%{tx.score}',msg:'aaa'")";
    Antlr4::Parser parser;
    auto result = parser.load(rule_directive);
    ASSERT_TRUE(result.has_value());
    auto& actions = parser.rules().back()->actions();
    EXPECT_EQ(actions.size(), 1);
    actions.back()->evaluate(*t);
    int score2 = std::get<int>(t->getVariable("score2"));
    int score = std::get<int>(t->getVariable("score"));
    const std::string& foo = std::get<std::string>(t->getVariable("foo2"));
    EXPECT_EQ(foo, std::format("{}_{}", score2, score));
  }

  // Remove
  {
    EXPECT_FALSE(IS_EMPTY_VARIANT(t->getVariable("score2")));
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:7,setvar:'!tx.score2',msg:'aaa'")";
    Antlr4::Parser parser;
    auto result = parser.load(rule_directive);
    ASSERT_TRUE(result.has_value());
    auto& actions = parser.rules().back()->actions();
    EXPECT_EQ(actions.size(), 1);
    actions.back()->evaluate(*t);
    EXPECT_TRUE(IS_EMPTY_VARIANT(t->getVariable("score2")));
  }

  // Remove (Macro expansion)
  {
    EXPECT_FALSE(IS_EMPTY_VARIANT(t->getVariable("score_bar")));
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:8,setvar:'!tx.score_%{tx.foo}',msg:'aaa'")";
    Antlr4::Parser parser;
    auto result = parser.load(rule_directive);
    ASSERT_TRUE(result.has_value());
    auto& actions = parser.rules().back()->actions();
    EXPECT_EQ(actions.size(), 1);
    actions.back()->evaluate(*t);
    EXPECT_TRUE(IS_EMPTY_VARIANT(t->getVariable("score_bar")));
  }

  // Increase
  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:9,setvar:'tx.score1=100',msg:'aaa'"
        SecRule ARGS:aaa|ARGS:bbb "bar" "id:10,setvar:'tx.score1=+100',msg:'aaa'"
        SecRule ARGS:aaa|ARGS:bbb "bar" "id:11,setvar:'tx.score200=200',msg:'aaa'")";
    Antlr4::Parser parser;
    auto result = parser.load(rule_directive);
    ASSERT_TRUE(result.has_value());
    for (auto& rule : parser.rules()) {
      auto& actions = rule->actions();
      EXPECT_EQ(actions.size(), 1);
      actions.back()->evaluate(*t);
    }
    EXPECT_EQ(std::get<int>(t->getVariable("score1")), 200);
    EXPECT_EQ(std::get<int>(t->getVariable("score200")), 200);
  }

  // Increase (value macro expansion)
  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:12,setvar:'tx.score%{tx.score200}=+%{tx.score1}',msg:'aaa'")";
    Antlr4::Parser parser;
    auto result = parser.load(rule_directive);
    ASSERT_TRUE(result.has_value());
    auto& actions = parser.rules().back()->actions();
    EXPECT_EQ(actions.size(), 1);
    actions.back()->evaluate(*t);
    EXPECT_EQ(std::get<int>(t->getVariable("score200")), 400);
  }

  // Decrease
  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:13,setvar:'tx.score2=350',msg:'aaa'"
        SecRule ARGS:aaa|ARGS:bbb "bar" "id:14,setvar:'tx.score2=-50',msg:'aaa'"
        SecRule ARGS:aaa|ARGS:bbb "bar" "id:15,setvar:'tx.score300=300',msg:'aaa'")";
    Antlr4::Parser parser;
    auto result = parser.load(rule_directive);
    ASSERT_TRUE(result.has_value());
    for (auto& rule : parser.rules()) {
      auto& actions = rule->actions();
      EXPECT_EQ(actions.size(), 1);
      actions.back()->evaluate(*t);
    }
    EXPECT_EQ(std::get<int>(t->getVariable("score2")), 300);
    EXPECT_EQ(std::get<int>(t->getVariable("score300")), 300);
  }

  // Decrease (value macro expansion)
  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:5,setvar:'tx.score%{tx.score300}=-%{tx.score1}',msg:'aaa'")";
    Antlr4::Parser parser;
    auto result = parser.load(rule_directive);
    ASSERT_TRUE(result.has_value());
    auto& actions = parser.rules().back()->actions();
    EXPECT_EQ(actions.size(), 1);
    actions.back()->evaluate(*t);
    EXPECT_EQ(std::get<int>(t->getVariable("score1")), 200);
    EXPECT_EQ(std::get<int>(t->getVariable("score300")), 100);
  }
}

TEST_F(RuleTest, ActionSetVarWithNoSigleQuote) {
  auto t = engine_.makeTransaction();

  // Create
  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,setvar:tx.score,msg:'aaa'")";
    Antlr4::Parser parser;
    auto result = parser.load(rule_directive);
    ASSERT_TRUE(result.has_value());
    auto& actions = parser.rules().back()->actions();
    EXPECT_EQ(actions.size(), 1);
    actions.back()->evaluate(*t);
    int score = std::get<int>(t->getVariable("score"));
    EXPECT_EQ(score, 1);
  }

  // Create and init
  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:2,setvar:tx.score2=100,msg:'aaa'")";
    Antlr4::Parser parser;
    auto result = parser.load(rule_directive);
    ASSERT_TRUE(result.has_value());
    ;
    auto& actions = parser.rules().back()->actions();
    EXPECT_EQ(actions.size(), 1);
    actions.back()->evaluate(*t);
    int score = std::get<int>(t->getVariable("score2"));
    EXPECT_EQ(score, 100);
  }

  // Create and init (Macro expansion)
  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:2,setvar:tx.score3=%{tx.score2},msg:'aaa'")";
    Antlr4::Parser parser;
    auto result = parser.load(rule_directive);
    ASSERT_TRUE(result.has_value());
    auto& actions = parser.rules().back()->actions();
    EXPECT_EQ(actions.size(), 1);
    actions.back()->evaluate(*t);
    int score2 = std::get<int>(t->getVariable("score2"));
    int score3 = std::get<int>(t->getVariable("score3"));
    EXPECT_EQ(score2, score3);
  }

  // Create and init (Multi macro expansion)
  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:2,setvar:tx.foo=%{tx.score2}_%{tx.score},msg:'aaa'")";
    Antlr4::Parser parser;
    auto result = parser.load(rule_directive);
    ASSERT_TRUE(result.has_value());
    auto& actions = parser.rules().back()->actions();
    EXPECT_EQ(actions.size(), 1);
    actions.back()->evaluate(*t);
    int score2 = std::get<int>(t->getVariable("score2"));
    int score = std::get<int>(t->getVariable("score"));
    const std::string& foo = std::get<std::string>(t->getVariable("foo"));
    EXPECT_EQ(foo, std::format("{}_{}", score2, score));
  }

  // Remove
  {
    EXPECT_FALSE(IS_EMPTY_VARIANT(t->getVariable("score2")));
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:3,setvar:!tx.score2,msg:'aaa'")";
    Antlr4::Parser parser;
    auto result = parser.load(rule_directive);
    ASSERT_TRUE(result.has_value());
    auto& actions = parser.rules().back()->actions();
    EXPECT_EQ(actions.size(), 1);
    actions.back()->evaluate(*t);
    EXPECT_TRUE(IS_EMPTY_VARIANT(t->getVariable("score2")));
  }

  // Increase
  {
    int old_score = std::get<int>(t->getVariable("score"));
    EXPECT_NE(old_score, 0);
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:4,setvar:tx.score=+100,msg:'aaa'")";
    Antlr4::Parser parser;
    auto result = parser.load(rule_directive);
    ASSERT_TRUE(result.has_value());
    auto& actions = parser.rules().back()->actions();
    EXPECT_EQ(actions.size(), 1);
    actions.back()->evaluate(*t);
    int new_score = std::get<int>(t->getVariable("score"));
    EXPECT_EQ(new_score, old_score + 100);
  }

  // Increase (Macro expansion)
  {
    int old_score = std::get<int>(t->getVariable("score"));
    EXPECT_NE(old_score, 0);
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:4,setvar:tx.score=+%{tx.score},msg:'aaa'")";
    Antlr4::Parser parser;
    auto result = parser.load(rule_directive);
    ASSERT_TRUE(result.has_value());
    auto& actions = parser.rules().back()->actions();
    EXPECT_EQ(actions.size(), 1);
    actions.back()->evaluate(*t);
    int new_score = std::get<int>(t->getVariable("score"));
    EXPECT_EQ(old_score, new_score - old_score);
  }

  // Decrease
  {
    int old_score = std::get<int>(t->getVariable("score"));
    EXPECT_NE(old_score, 0);
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:5,setvar:tx.score=-50,msg:'aaa'")";
    Antlr4::Parser parser;
    auto result = parser.load(rule_directive);
    ASSERT_TRUE(result.has_value());
    auto& actions = parser.rules().back()->actions();
    EXPECT_EQ(actions.size(), 1);
    actions.back()->evaluate(*t);
    int new_score = std::get<int>(t->getVariable("score"));
    EXPECT_EQ(old_score, new_score + 50);
  }

  // Decrease (Macro expansion)
  {
    int old_score = std::get<int>(t->getVariable("score"));
    EXPECT_NE(old_score, 0);
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:5,setvar:tx.score=-%{tx.score},msg:'aaa'")";
    Antlr4::Parser parser;
    auto result = parser.load(rule_directive);
    ASSERT_TRUE(result.has_value());
    auto& actions = parser.rules().back()->actions();
    EXPECT_EQ(actions.size(), 1);
    actions.back()->evaluate(*t);
    int new_score = std::get<int>(t->getVariable("score"));
    EXPECT_EQ(old_score, new_score + old_score);
  }
}

TEST_F(RuleTest, ActionSetEnv) {
  auto t = engine_.makeTransaction();
  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,setenv:'var1=hello',msg:'aaa bbb'")";
    Antlr4::Parser parser;
    auto result = parser.load(rule_directive);
    ASSERT_TRUE(result.has_value());
    auto& actions = parser.rules().back()->actions();
    EXPECT_EQ(actions.size(), 1);
    actions.back()->evaluate(*t);
    EXPECT_EQ(std::string("hello"), ::getenv("var1"));
  }
}

TEST_F(RuleTest, ActionSetRsc) {
  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,setrsc:'this is rsc',msg:'aaa'")";
    Antlr4::Parser parser;
    auto result = parser.load(rule_directive);
    ASSERT_TRUE(result.has_value());
    auto& actions = parser.rules().back()->actions();
    EXPECT_EQ(actions.size(), 1);
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
  }
}

TEST_F(RuleTest, ActionSetSid) {
  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,setsid:'this is sid',msg:'aaa'")";
    Antlr4::Parser parser;
    auto result = parser.load(rule_directive);
    ASSERT_TRUE(result.has_value());
    auto& actions = parser.rules().back()->actions();
    EXPECT_EQ(actions.size(), 1);
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
  }
}

TEST_F(RuleTest, ActionSetUid) {
  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,setuid:'this is uid',msg:'aaa'")";
    Antlr4::Parser parser;
    auto result = parser.load(rule_directive);
    ASSERT_TRUE(result.has_value());
    auto& actions = parser.rules().back()->actions();
    EXPECT_EQ(actions.size(), 1);
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
  }
}

TEST_F(RuleTest, ActionTransformation) {
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

TEST_F(RuleTest, ActionAuditLog) {
  const std::string rule_directive = R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,auditlog,msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());
  EXPECT_TRUE(parser.rules().back()->auditLog());
}

TEST_F(RuleTest, ActionLog) {
  const std::string rule_directive = R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,log,msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());
  EXPECT_TRUE(parser.rules().back()->log());
}

TEST_F(RuleTest, ActionNoAuditLog) {
  const std::string rule_directive =
      R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,noauditlog,msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());
  EXPECT_FALSE(parser.rules().back()->auditLog().value_or(true));
}

TEST_F(RuleTest, ActionNoLog) {
  const std::string rule_directive = R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,nolog,msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());
  EXPECT_FALSE(parser.rules().back()->log().value_or(true));
}

TEST_F(RuleTest, ActionCapture) {
  const std::string rule_directive = R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,capture,msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());
  EXPECT_TRUE(parser.rules().back()->capture());
}

TEST_F(RuleTest, ActionMultiMatch) {
  const std::string rule_directive =
      R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,multiMatch,msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());
  EXPECT_TRUE(parser.rules().back()->multiMatch());
}

TEST_F(RuleTest, ActionAllow) {
  const std::string rule_directive = R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,allow,msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(parser.rules().back()->disruptive(), Rule::Disruptive::ALLOW);
}

TEST_F(RuleTest, ActionBlock) {
  const std::string rule_directive = R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,block,msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(parser.rules().back()->disruptive(), Rule::Disruptive::BLOCK);
}

TEST_F(RuleTest, ActionDeny) {
  const std::string rule_directive = R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,deny,msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(parser.rules().back()->disruptive(), Rule::Disruptive::DENY);
}

TEST_F(RuleTest, ActionDrop) {
  const std::string rule_directive = R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,drop,msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(parser.rules().back()->disruptive(), Rule::Disruptive::DROP);
}

TEST_F(RuleTest, ActionPass) {
  const std::string rule_directive = R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,pass,msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(parser.rules().back()->disruptive(), Rule::Disruptive::PASS);
}

TEST_F(RuleTest, ActionRedirect) {
  const std::string rule_directive =
      R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,redirect:http://www.srhino.com,msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(parser.rules().back()->disruptive(), Rule::Disruptive::REDIRECT);
  EXPECT_EQ(parser.rules().back()->redirect(), "http://www.srhino.com");
}

TEST_F(RuleTest, ActionStatus) {
  const std::string rule_directive =
      R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,status:500,msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(parser.rules().back()->status(), "500");
}

TEST_F(RuleTest, ActionXmlns) {
  const std::string rule_directive =
      R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,xmlns:xsd=http://www.w3.org/2001/XMLSchema,msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(parser.rules().back()->xmlns(), "xsd=http://www.w3.org/2001/XMLSchema");
}

TEST_F(RuleTest, ActionCtlAuditEngine) {
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

TEST_F(RuleTest, ActionCtlAuditLogParts) {
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

TEST_F(RuleTest, ActionCtlRequestBodyAccess) {
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

TEST_F(RuleTest, ActionCtlRequestBodyProcessor) {
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

TEST_F(RuleTest, ActionCtlRuleEngine) {
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

TEST_F(RuleTest, ActionCtlRuleRemoveById) {
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

TEST_F(RuleTest, ActionCtlRuleRemoveByTag) {
  const std::string rule_directive =
      R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,ctl:ruleRemoveByTag=foo,msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());
}

TEST_F(RuleTest, ActionCtlRuleRemoveTargetById) {
  const std::string rule_directive =
      R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,ctl:ruleRemoveTargetById=123;ARGS:foo|ARGS:bar,msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());
}

TEST_F(RuleTest, ActionCtlRuleRemoveTargetByTag) {
  const std::string rule_directive =
      R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,ctl:ruleRemoveTargetByTag=foo;ARGS:foo|ARGS:bar,msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());
}

TEST_F(RuleTest, ActionChain) {
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
  EXPECT_EQ(rule_var_pool.size(), 4);
  EXPECT_NE(nullptr, dynamic_cast<Variable::ArgsGet*>(rule_var_pool[0].get()));
  EXPECT_EQ(rule_var_pool[0]->subName(), "");
  EXPECT_FALSE(rule_var_pool[0]->isCounter());
  EXPECT_FALSE(rule_var_pool[0]->isNot());

  EXPECT_NE(nullptr, dynamic_cast<Variable::ArgsPost*>(rule_var_pool[1].get()));
  EXPECT_EQ(rule_var_pool[1]->subName(), "foo");
  EXPECT_FALSE(rule_var_pool[1]->isCounter());
  EXPECT_FALSE(rule_var_pool[1]->isNot());

  EXPECT_NE(nullptr, dynamic_cast<Variable::ArgsGet*>(rule_var_pool[2].get()));
  EXPECT_EQ(rule_var_pool[2]->subName(), "foo");
  EXPECT_FALSE(rule_var_pool[2]->isCounter());
  EXPECT_TRUE(rule_var_pool[2]->isNot());

  EXPECT_NE(nullptr, dynamic_cast<Variable::Args*>(rule_var_pool[3].get()));
  EXPECT_EQ(rule_var_pool[3]->subName(), "");
  EXPECT_TRUE(rule_var_pool[3]->isCounter());
  EXPECT_FALSE(rule_var_pool[3]->isNot());

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

TEST_F(RuleTest, ActionInitCol) {
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

TEST_F(RuleTest, ActionSkipAfter) {
  const std::string rule_directive =
      R"(SecRule ARGS:aaa|ARGS:bbb "foo" "id:1,skipAfter:hi,msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(parser.rules().back()->skipAfter(), "hi");
}

TEST_F(RuleTest, ActionSkip) {
  const std::string rule_directive = R"(SecRule ARGS:aaa|ARGS:bbb "foo" "id:1,skip:3,msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(parser.rules().back()->skip(), 3);
}

TEST_F(RuleTest, ActionServerity) {
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

TEST_F(RuleTest, ActionIdWithString) {
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
  ASSERT_EQ(rule_var_pool.size(), 4);
  EXPECT_NE(nullptr, dynamic_cast<Variable::ArgsGet*>(rule_var_pool[0].get()));
  EXPECT_EQ(rule_var_pool[0]->subName(), "");
  EXPECT_FALSE(rule_var_pool[0]->isCounter());
  EXPECT_FALSE(rule_var_pool[0]->isNot());

  EXPECT_NE(nullptr, dynamic_cast<Variable::ArgsPost*>(rule_var_pool[1].get()));
  EXPECT_EQ(rule_var_pool[1]->subName(), "foo");
  EXPECT_FALSE(rule_var_pool[1]->isCounter());
  EXPECT_FALSE(rule_var_pool[1]->isNot());

  EXPECT_NE(nullptr, dynamic_cast<Variable::ArgsGet*>(rule_var_pool[2].get()));
  EXPECT_EQ(rule_var_pool[2]->subName(), "foo");
  EXPECT_FALSE(rule_var_pool[2]->isCounter());
  EXPECT_TRUE(rule_var_pool[2]->isNot());

  EXPECT_NE(nullptr, dynamic_cast<Variable::Args*>(rule_var_pool[3].get()));
  EXPECT_EQ(rule_var_pool[3]->subName(), "");
  EXPECT_TRUE(rule_var_pool[3]->isCounter());
  EXPECT_FALSE(rule_var_pool[3]->isNot());

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

TEST_F(RuleTest, ActionMsgWithMacro) {
  const std::string rule_directive =
      R"(SecRule ARGS_GET|ARGS_POST:foo|!ARGS_GET:foo|&ARGS "bar" "id:'111',tag:'foo',msg:'foo: %{tx.foo} bar: %{tx.bar}'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);

  ASSERT_TRUE(result.has_value());

  EXPECT_TRUE(parser.rules().back()->msg().empty());
}

TEST_F(RuleTest, ActionLogData) {
  const std::string rule_directive =
      R"(SecRule ARGS:aaa|ARGS:bbb "foo" "id:1,logdata:'this is logdata',msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(parser.rules().back()->logdata(), "this is logdata");
}

TEST_F(RuleTest, ActionLogDataWithMacro) {
  const std::string rule_directive =
      R"(SecRule ARGS:aaa|ARGS:bbb "foo" "id:1,logdata:'foo: %{tx.foo} bar: %{tx.bar}',msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());

  EXPECT_TRUE(parser.rules().back()->logdata().empty());
}
} // namespace Parser
} // namespace SrSecurity