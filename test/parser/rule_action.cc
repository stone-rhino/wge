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
class RuleActionTest : public testing::Test {};

TEST_F(RuleActionTest, NoAction) {
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

TEST_F(RuleActionTest, ActionSetVar) {
  // Create
  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,setvar:'tx.score',msg:'aaa'")";

    Engine engine(spdlog::level::trace);
    auto result = engine.load(rule_directive);
    engine.init();
    auto t = engine.makeTransaction();
    ASSERT_TRUE(result.has_value());

    auto& actions = engine.rules(1).back()->actions();
    EXPECT_EQ(actions.size(), 1);
    actions.back()->evaluate(*t);
    int score = std::get<int>(t->getVariable("score"));
    EXPECT_EQ(score, 1);
  }

  // Create (Macro expansion)
  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,setvar:'tx.foo=bar',msg:'aaa'"
        SecRule ARGS:aaa|ARGS:bbb "bar" "id:2,phase:1,setvar:'tx.%{tx.foo}score',msg:'aaa'")";

    Engine engine(spdlog::level::trace);
    auto result = engine.load(rule_directive);
    engine.init();
    auto t = engine.makeTransaction();
    ASSERT_TRUE(result.has_value());

    for (auto rule : engine.rules(1)) {
      for (auto& action : rule->actions()) {
        action->evaluate(*t);
      }
    }
    EXPECT_EQ(std::get<std::string_view>(t->getVariable("foo")), "bar");
    int score = std::get<int>(t->getVariable("barscore"));
    EXPECT_EQ(score, 1);
  }

  // Create and init
  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,setvar:'tx.score2=100',msg:'aaa'")";

    Engine engine(spdlog::level::trace);
    auto result = engine.load(rule_directive);
    engine.init();
    auto t = engine.makeTransaction();
    ASSERT_TRUE(result.has_value());

    auto& actions = engine.rules(1).back()->actions();
    EXPECT_EQ(actions.size(), 1);
    actions.back()->evaluate(*t);
    int score = std::get<int>(t->getVariable("score2"));
    EXPECT_EQ(score, 100);
  }

  // Create and init (Macro expansion)
  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,setvar:'tx.foo=bar',msg:'aaa'"
        SecRule ARGS:aaa|ARGS:bbb "bar" "id:2,phase:1,setvar:'tx.score2=100',msg:'aaa'"
        SecRule ARGS:aaa|ARGS:bbb "bar" "id:3,phase:1,setvar:'tx.score_%{tx.foo}=%{tx.score2}',msg:'aaa'")";

    Engine engine(spdlog::level::trace);
    auto result = engine.load(rule_directive);
    engine.init();
    auto t = engine.makeTransaction();
    ASSERT_TRUE(result.has_value());

    for (auto rule : engine.rules(1)) {
      for (auto& action : rule->actions()) {
        action->evaluate(*t);
      }
    }
    int score2 = std::get<int>(t->getVariable("score2"));
    int score_bar = std::get<int>(t->getVariable("score_bar"));
    EXPECT_EQ(score2, score_bar);
  }

  // Create and init (Multi macro expansion)
  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,setvar:'tx.score',setvar:'tx.score2=100',msg:'aaa'"
        SecRule ARGS:aaa|ARGS:bbb "bar" "id:2,phase:1,setvar:'tx.foo2=%{tx.score2}_%{tx.score}',msg:'aaa'")";

    Engine engine(spdlog::level::trace);
    auto result = engine.load(rule_directive);
    engine.init();
    auto t = engine.makeTransaction();
    ASSERT_TRUE(result.has_value());

    for (auto rule : engine.rules(1)) {
      for (auto& action : rule->actions()) {
        action->evaluate(*t);
      }
    }
    int score2 = std::get<int>(t->getVariable("score2"));
    int score = std::get<int>(t->getVariable("score"));
    auto foo = std::get<std::string_view>(t->getVariable("foo2"));
    EXPECT_EQ(foo, std::format("{}_{}", score2, score));
  }

  // Remove
  {
    const std::string rule_directive1 =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,setvar:'tx.score2',msg:'aaa'")";

    const std::string rule_directive2 =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:2,phase:1,setvar:'!tx.score2',msg:'aaa'")";

    Engine engine(spdlog::level::trace);
    auto result = engine.load(rule_directive1);
    engine.init();
    auto t = engine.makeTransaction();
    ASSERT_TRUE(result.has_value());

    auto& actions1 = engine.rules(1).back()->actions();
    EXPECT_EQ(actions1.size(), 1);
    actions1.back()->evaluate(*t);
    EXPECT_EQ(std::get<int>(t->getVariable("score2")), 1);

    result = engine.load(rule_directive2);
    engine.init();
    auto& actions2 = engine.rules(1).back()->actions();
    EXPECT_EQ(actions2.size(), 1);
    actions2.back()->evaluate(*t);
    EXPECT_FALSE(t->hasVariable("score2"));
  }

  // Remove (Macro expansion)
  {
    const std::string rule_directive1 =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,setvar:'tx.foo=bar',setvar:'tx.score_bar',msg:'aaa'")";

    const std::string rule_directive2 =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:2,phase:1,setvar:'!tx.score_%{tx.foo}',msg:'aaa'")";

    Engine engine(spdlog::level::trace);
    auto result = engine.load(rule_directive1);
    engine.init();
    auto t = engine.makeTransaction();
    ASSERT_TRUE(result.has_value());

    auto& actions1 = engine.rules(1).back()->actions();
    for (auto& action : actions1) {
      action->evaluate(*t);
    }
    EXPECT_EQ(std::get<std::string_view>(t->getVariable("foo")), "bar");
    EXPECT_EQ(std::get<int>(t->getVariable("score_bar")), 1);

    result = engine.load(rule_directive2);
    engine.init();
    auto& actions2 = engine.rules(1).back()->actions();
    for (auto& action : actions2) {
      action->evaluate(*t);
    }
    EXPECT_FALSE(t->hasVariable("score_bar"));
  }

  // Increase
  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,setvar:'tx.score1=100',msg:'aaa'"
        SecRule ARGS:aaa|ARGS:bbb "bar" "id:2,phase:1,setvar:'tx.score1=+100',msg:'aaa'")";

    Engine engine(spdlog::level::trace);
    auto result = engine.load(rule_directive);
    engine.init();
    auto t = engine.makeTransaction();
    ASSERT_TRUE(result.has_value());

    for (auto& rule : engine.rules(1)) {
      auto& actions = rule->actions();
      EXPECT_EQ(actions.size(), 1);
      actions.back()->evaluate(*t);
    }
    EXPECT_EQ(std::get<int>(t->getVariable("score1")), 200);
  }

  // Increase (value macro expansion)
  {
    const std::string rule_directive1 =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,setvar:'tx.score200=200',setvar:'tx.score=100',msg:'aaa'")";

    const std::string rule_directive2 =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:2,phase:1,setvar:'tx.score%{tx.score200}=+%{tx.score}',msg:'aaa'")";

    Engine engine(spdlog::level::trace);
    auto result = engine.load(rule_directive1);
    engine.init();
    auto t = engine.makeTransaction();
    ASSERT_TRUE(result.has_value());

    auto& actions1 = engine.rules(1).back()->actions();
    for (auto& action : actions1) {
      action->evaluate(*t);
    }
    EXPECT_EQ(std::get<int>(t->getVariable("score200")), 200);
    EXPECT_EQ(std::get<int>(t->getVariable("score")), 100);

    result = engine.load(rule_directive2);
    engine.init();
    ASSERT_TRUE(result.has_value());

    auto& actions2 = engine.rules(1).back()->actions();
    for (auto& action : actions2) {
      action->evaluate(*t);
    }
    EXPECT_EQ(std::get<int>(t->getVariable("score200")), 300);
    EXPECT_EQ(std::get<int>(t->getVariable("score")), 100);
  }

  // Decrease
  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,setvar:'tx.score1=100',msg:'aaa'"
    SecRule ARGS:aaa|ARGS:bbb "bar" "id:2,phase:1,setvar:'tx.score1=-50',msg:'aaa'")";

    Engine engine(spdlog::level::trace);
    auto result = engine.load(rule_directive);
    engine.init();
    auto t = engine.makeTransaction();
    ASSERT_TRUE(result.has_value());

    for (auto& rule : engine.rules(1)) {
      auto& actions = rule->actions();
      EXPECT_EQ(actions.size(), 1);
      actions.back()->evaluate(*t);
    }
    EXPECT_EQ(std::get<int>(t->getVariable("score1")), 50);
  }

  // Decrease (value macro expansion)
  {
    const std::string rule_directive1 =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,setvar:'tx.score200=200',setvar:'tx.score=100',msg:'aaa'")";

    const std::string rule_directive2 =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:2,phase:1,setvar:'tx.score%{tx.score200}=-%{tx.score}',msg:'aaa'")";

    Engine engine(spdlog::level::trace);
    auto result = engine.load(rule_directive1);
    engine.init();
    auto t = engine.makeTransaction();
    ASSERT_TRUE(result.has_value());

    auto& actions1 = engine.rules(1).back()->actions();
    for (auto& action : actions1) {
      action->evaluate(*t);
    }
    EXPECT_EQ(std::get<int>(t->getVariable("score200")), 200);
    EXPECT_EQ(std::get<int>(t->getVariable("score")), 100);

    result = engine.load(rule_directive2);
    engine.init();
    ASSERT_TRUE(result.has_value());

    auto& actions2 = engine.rules(1).back()->actions();
    for (auto& action : actions2) {
      action->evaluate(*t);
    }
    EXPECT_EQ(std::get<int>(t->getVariable("score200")), 100);
    EXPECT_EQ(std::get<int>(t->getVariable("score")), 100);
  }
}

TEST_F(RuleActionTest, ActionSetVarWithNoSigleQuote) {
  // Create
  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,setvar:tx.score,msg:'aaa'")";

    Engine engine(spdlog::level::trace);
    auto result = engine.load(rule_directive);
    engine.init();
    auto t = engine.makeTransaction();
    ASSERT_TRUE(result.has_value());

    auto& actions = engine.rules(1).back()->actions();
    EXPECT_EQ(actions.size(), 1);
    actions.back()->evaluate(*t);
    int score = std::get<int>(t->getVariable("score"));
    EXPECT_EQ(score, 1);
  }

  // Create (Macro expansion)
  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,setvar:tx.foo=bar,msg:'aaa'"
          SecRule ARGS:aaa|ARGS:bbb "bar" "id:2,phase:1,setvar:tx.%{tx.foo}score,msg:'aaa'")";

    Engine engine(spdlog::level::trace);
    auto result = engine.load(rule_directive);
    engine.init();
    auto t = engine.makeTransaction();
    ASSERT_TRUE(result.has_value());

    for (auto rule : engine.rules(1)) {
      for (auto& action : rule->actions()) {
        action->evaluate(*t);
      }
    }
    EXPECT_EQ(std::get<std::string_view>(t->getVariable("foo")), "bar");
    int score = std::get<int>(t->getVariable("barscore"));
    EXPECT_EQ(score, 1);
  }

  // Create and init
  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,setvar:tx.score2=100,msg:'aaa'")";

    Engine engine(spdlog::level::trace);
    auto result = engine.load(rule_directive);
    engine.init();
    auto t = engine.makeTransaction();
    ASSERT_TRUE(result.has_value());

    auto& actions = engine.rules(1).back()->actions();
    EXPECT_EQ(actions.size(), 1);
    actions.back()->evaluate(*t);
    int score = std::get<int>(t->getVariable("score2"));
    EXPECT_EQ(score, 100);
  }

  // Create and init (Macro expansion)
  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,setvar:tx.foo=bar,msg:'aaa'"
          SecRule ARGS:aaa|ARGS:bbb "bar" "id:2,phase:1,setvar:tx.score2=100,msg:'aaa'"
          SecRule ARGS:aaa|ARGS:bbb "bar" "id:3,phase:1,setvar:tx.score_%{tx.foo}=%{tx.score2},msg:'aaa'")";

    Engine engine(spdlog::level::trace);
    auto result = engine.load(rule_directive);
    engine.init();
    auto t = engine.makeTransaction();
    ASSERT_TRUE(result.has_value());

    for (auto rule : engine.rules(1)) {
      for (auto& action : rule->actions()) {
        action->evaluate(*t);
      }
    }
    int score2 = std::get<int>(t->getVariable("score2"));
    int score_bar = std::get<int>(t->getVariable("score_bar"));
    EXPECT_EQ(score2, score_bar);
  }

  // Create and init (Multi macro expansion)
  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,setvar:tx.score,setvar:tx.score2=100,msg:'aaa'"
          SecRule ARGS:aaa|ARGS:bbb "bar" "id:2,phase:1,setvar:tx.foo2=%{tx.score2}_%{tx.score},msg:'aaa'")";

    Engine engine(spdlog::level::trace);
    auto result = engine.load(rule_directive);
    engine.init();
    auto t = engine.makeTransaction();
    ASSERT_TRUE(result.has_value());

    for (auto rule : engine.rules(1)) {
      for (auto& action : rule->actions()) {
        action->evaluate(*t);
      }
    }
    int score2 = std::get<int>(t->getVariable("score2"));
    int score = std::get<int>(t->getVariable("score"));
    auto foo = std::get<std::string_view>(t->getVariable("foo2"));
    EXPECT_EQ(foo, std::format("{}_{}", score2, score));
  }

  // Remove
  {
    const std::string rule_directive1 =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,setvar:tx.score2,msg:'aaa'")";

    const std::string rule_directive2 =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:2,phase:1,setvar:!tx.score2,msg:'aaa'")";

    Engine engine(spdlog::level::trace);
    auto result = engine.load(rule_directive1);
    engine.init();
    auto t = engine.makeTransaction();
    ASSERT_TRUE(result.has_value());

    auto& actions1 = engine.rules(1).back()->actions();
    EXPECT_EQ(actions1.size(), 1);
    actions1.back()->evaluate(*t);
    EXPECT_EQ(std::get<int>(t->getVariable("score2")), 1);

    result = engine.load(rule_directive2);
    engine.init();
    auto& actions2 = engine.rules(1).back()->actions();
    EXPECT_EQ(actions2.size(), 1);
    actions2.back()->evaluate(*t);
    EXPECT_FALSE(t->hasVariable("score2"));
  }

  // Remove (Macro expansion)
  {
    const std::string rule_directive1 =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,setvar:'tx.foo=bar',setvar:tx.score_bar,msg:'aaa'")";

    const std::string rule_directive2 =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:2,phase:1,setvar:!tx.score_%{tx.foo},msg:'aaa'")";

    Engine engine(spdlog::level::trace);
    auto result = engine.load(rule_directive1);
    engine.init();
    auto t = engine.makeTransaction();
    ASSERT_TRUE(result.has_value());

    auto& actions1 = engine.rules(1).back()->actions();
    for (auto& action : actions1) {
      action->evaluate(*t);
    }
    EXPECT_EQ(std::get<std::string_view>(t->getVariable("foo")), "bar");
    EXPECT_EQ(std::get<int>(t->getVariable("score_bar")), 1);

    result = engine.load(rule_directive2);
    engine.init();
    auto& actions2 = engine.rules(1).back()->actions();
    for (auto& action : actions2) {
      action->evaluate(*t);
    }
    EXPECT_FALSE(t->hasVariable("score_bar"));
  }

  // Increase
  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,setvar:tx.score1=100,msg:'aaa'"
          SecRule ARGS:aaa|ARGS:bbb "bar" "id:2,phase:1,setvar:tx.score1=+100,msg:'aaa'")";

    Engine engine(spdlog::level::trace);
    auto result = engine.load(rule_directive);
    engine.init();
    auto t = engine.makeTransaction();
    ASSERT_TRUE(result.has_value());

    for (auto& rule : engine.rules(1)) {
      auto& actions = rule->actions();
      EXPECT_EQ(actions.size(), 1);
      actions.back()->evaluate(*t);
    }
    EXPECT_EQ(std::get<int>(t->getVariable("score1")), 200);
  }

  // Increase (value macro expansion)
  {
    const std::string rule_directive1 =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,setvar:tx.score200=200,setvar:'tx.score=100',msg:'aaa'")";

    const std::string rule_directive2 =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:2,phase:1,setvar:tx.score%{tx.score200}=+%{tx.score},msg:'aaa'")";

    Engine engine(spdlog::level::trace);
    auto result = engine.load(rule_directive1);
    engine.init();
    auto t = engine.makeTransaction();
    ASSERT_TRUE(result.has_value());

    auto& actions1 = engine.rules(1).back()->actions();
    for (auto& action : actions1) {
      action->evaluate(*t);
    }
    EXPECT_EQ(std::get<int>(t->getVariable("score200")), 200);
    EXPECT_EQ(std::get<int>(t->getVariable("score")), 100);

    result = engine.load(rule_directive2);
    engine.init();
    ASSERT_TRUE(result.has_value());

    auto& actions2 = engine.rules(1).back()->actions();
    for (auto& action : actions2) {
      action->evaluate(*t);
    }
    EXPECT_EQ(std::get<int>(t->getVariable("score200")), 300);
    EXPECT_EQ(std::get<int>(t->getVariable("score")), 100);
  }

  // Decrease
  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,setvar:tx.score1=100,msg:'aaa'"
      SecRule ARGS:aaa|ARGS:bbb "bar" "id:2,phase:1,setvar:tx.score1=-50,msg:'aaa'")";

    Engine engine(spdlog::level::trace);
    auto result = engine.load(rule_directive);
    engine.init();
    auto t = engine.makeTransaction();
    ASSERT_TRUE(result.has_value());

    for (auto& rule : engine.rules(1)) {
      auto& actions = rule->actions();
      EXPECT_EQ(actions.size(), 1);
      actions.back()->evaluate(*t);
    }
    EXPECT_EQ(std::get<int>(t->getVariable("score1")), 50);
  }

  // Decrease (value macro expansion)
  {
    const std::string rule_directive1 =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,setvar:tx.score200=200,setvar:'tx.score=100',msg:'aaa'")";

    const std::string rule_directive2 =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:2,phase:1,setvar:tx.score%{tx.score200}=-%{tx.score},msg:'aaa'")";

    Engine engine(spdlog::level::trace);
    auto result = engine.load(rule_directive1);
    engine.init();
    auto t = engine.makeTransaction();
    ASSERT_TRUE(result.has_value());

    auto& actions1 = engine.rules(1).back()->actions();
    for (auto& action : actions1) {
      action->evaluate(*t);
    }
    EXPECT_EQ(std::get<int>(t->getVariable("score200")), 200);
    EXPECT_EQ(std::get<int>(t->getVariable("score")), 100);

    result = engine.load(rule_directive2);
    engine.init();
    ASSERT_TRUE(result.has_value());

    auto& actions2 = engine.rules(1).back()->actions();
    for (auto& action : actions2) {
      action->evaluate(*t);
    }
    EXPECT_EQ(std::get<int>(t->getVariable("score200")), 100);
    EXPECT_EQ(std::get<int>(t->getVariable("score")), 100);
  }
}

TEST_F(RuleActionTest, ActionSetEnv) {
  const std::string rule_directive =
      R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,setenv:'var1=hello',msg:'aaa bbb'")";

  Engine engine(spdlog::level::trace);
  auto result = engine.load(rule_directive);
  engine.init();
  auto t = engine.makeTransaction();
  ASSERT_TRUE(result.has_value());

  auto& actions = engine.rules(1).back()->actions();
  EXPECT_EQ(actions.size(), 1);
  actions.back()->evaluate(*t);
  EXPECT_EQ(std::string("hello"), ::getenv("var1"));
}

TEST_F(RuleActionTest, ActionSetRsc) {
  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,setrsc:'this is rsc',msg:'aaa'")";

    Antlr4::Parser parser;
    auto result = parser.load(rule_directive);
    ASSERT_TRUE(result.has_value());
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

TEST_F(RuleActionTest, ActionSetSid) {
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

TEST_F(RuleActionTest, ActionSetUid) {
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

TEST_F(RuleActionTest, ActionTransformation) {
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

TEST_F(RuleActionTest, ActionAuditLog) {
  const std::string rule_directive = R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,auditlog,msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());
  EXPECT_TRUE(parser.rules().back()->auditLog());
}

TEST_F(RuleActionTest, ActionLog) {
  const std::string rule_directive = R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,log,msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());
  EXPECT_TRUE(parser.rules().back()->log());
}

TEST_F(RuleActionTest, ActionNoAuditLog) {
  const std::string rule_directive =
      R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,noauditlog,msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());
  EXPECT_FALSE(parser.rules().back()->auditLog().value_or(true));
}

TEST_F(RuleActionTest, ActionNoLog) {
  const std::string rule_directive = R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,nolog,msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());
  EXPECT_FALSE(parser.rules().back()->log().value_or(true));
}

TEST_F(RuleActionTest, ActionCapture) {
  const std::string rule_directive = R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,capture,msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());
  EXPECT_TRUE(parser.rules().back()->capture());
}

TEST_F(RuleActionTest, ActionMultiMatch) {
  const std::string rule_directive =
      R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,multiMatch,msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());
  EXPECT_TRUE(parser.rules().back()->multiMatch());
}

TEST_F(RuleActionTest, ActionAllow) {
  const std::string rule_directive = R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,allow,msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(parser.rules().back()->disruptive(), Rule::Disruptive::ALLOW);
}

TEST_F(RuleActionTest, ActionBlock) {
  const std::string rule_directive = R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,block,msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(parser.rules().back()->disruptive(), Rule::Disruptive::BLOCK);
}

TEST_F(RuleActionTest, ActionDeny) {
  const std::string rule_directive = R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,deny,msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(parser.rules().back()->disruptive(), Rule::Disruptive::DENY);
}

TEST_F(RuleActionTest, ActionDrop) {
  const std::string rule_directive = R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,drop,msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(parser.rules().back()->disruptive(), Rule::Disruptive::DROP);
}

TEST_F(RuleActionTest, ActionPass) {
  const std::string rule_directive = R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,pass,msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(parser.rules().back()->disruptive(), Rule::Disruptive::PASS);
}

TEST_F(RuleActionTest, ActionRedirect) {
  const std::string rule_directive =
      R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,redirect:http://www.srhino.com,msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(parser.rules().back()->disruptive(), Rule::Disruptive::REDIRECT);
  EXPECT_EQ(parser.rules().back()->redirect(), "http://www.srhino.com");
}

TEST_F(RuleActionTest, ActionStatus) {
  const std::string rule_directive =
      R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,status:500,msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(parser.rules().back()->status(), "500");
}

TEST_F(RuleActionTest, ActionXmlns) {
  const std::string rule_directive =
      R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,xmlns:xsd=http://www.w3.org/2001/XMLSchema,msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(parser.rules().back()->xmlns(), "xsd=http://www.w3.org/2001/XMLSchema");
}

TEST_F(RuleActionTest, ActionCtlAuditEngine) {
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

TEST_F(RuleActionTest, ActionCtlAuditLogParts) {
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

TEST_F(RuleActionTest, ActionCtlRequestBodyAccess) {
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

TEST_F(RuleActionTest, ActionCtlRequestBodyProcessor) {
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

TEST_F(RuleActionTest, ActionCtlRuleEngine) {
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

TEST_F(RuleActionTest, ActionCtlRuleRemoveById) {
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

TEST_F(RuleActionTest, ActionCtlRuleRemoveByTag) {
  const std::string rule_directive =
      R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,ctl:ruleRemoveByTag=foo,msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());
}

TEST_F(RuleActionTest, ActionCtlRuleRemoveTargetById) {
  const std::string rule_directive =
      R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,ctl:ruleRemoveTargetById=123;ARGS:foo|ARGS:bar,msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());
}

TEST_F(RuleActionTest, ActionCtlRuleRemoveTargetByTag) {
  const std::string rule_directive =
      R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,ctl:ruleRemoveTargetByTag=foo;ARGS:foo|ARGS:bar,msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());
}

TEST_F(RuleActionTest, ActionChain) {
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

TEST_F(RuleActionTest, ActionInitCol) {
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

TEST_F(RuleActionTest, ActionSkipAfter) {
  const std::string rule_directive =
      R"(SecRule ARGS:aaa|ARGS:bbb "foo" "id:1,skipAfter:hi,msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(parser.rules().back()->skipAfter(), "hi");
}

TEST_F(RuleActionTest, ActionSkip) {
  const std::string rule_directive = R"(SecRule ARGS:aaa|ARGS:bbb "foo" "id:1,skip:3,msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(parser.rules().back()->skip(), 3);
}

TEST_F(RuleActionTest, ActionServerity) {
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

TEST_F(RuleActionTest, ActionIdWithString) {
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

TEST_F(RuleActionTest, ActionMsgWithMacro) {
  const std::string rule_directive =
      R"(SecRule ARGS_GET|ARGS_POST:foo|!ARGS_GET:foo|&ARGS "bar" "id:'111',tag:'foo',msg:'foo: %{tx.foo} bar: %{tx.bar}'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);

  ASSERT_TRUE(result.has_value());

  EXPECT_TRUE(parser.rules().back()->msg().empty());
}

TEST_F(RuleActionTest, ActionLogData) {
  const std::string rule_directive =
      R"(SecRule ARGS:aaa|ARGS:bbb "foo" "id:1,logdata:'this is logdata',msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(parser.rules().back()->logdata(), "this is logdata");
}

TEST_F(RuleActionTest, ActionLogDataWithMacro) {
  const std::string rule_directive =
      R"(SecRule ARGS:aaa|ARGS:bbb "foo" "id:1,logdata:'foo: %{tx.foo} bar: %{tx.bar}',msg:'aaa'")";
  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());

  EXPECT_TRUE(parser.rules().back()->logdata().empty());
}
} // namespace Parser
} // namespace SrSecurity