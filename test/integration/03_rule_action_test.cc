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
namespace Integration {
class RuleActionTest : public testing::Test {};

TEST_F(RuleActionTest, ActionSetVar) {
  // Create
  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,setvar:'tx.score',msg:'aaa'")";

    Engine engine(spdlog::level::off);
    auto result = engine.load(rule_directive);
    engine.init();
    auto t = engine.makeTransaction();
    ASSERT_TRUE(result.has_value());

    auto& actions = engine.rules(1).back().actions();
    EXPECT_EQ(actions.size(), 1);
    actions.back()->evaluate(*t);
    int64_t score = std::get<int64_t>(t->getVariable("", "score"));
    EXPECT_EQ(score, 1);
  }

  // Create (Macro expansion)
  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,setvar:'tx.foo=bar',msg:'aaa'"
        SecRule ARGS:aaa|ARGS:bbb "bar" "id:2,phase:1,setvar:'tx.%{tx.foo}score',msg:'aaa'")";

    Engine engine(spdlog::level::off);
    auto result = engine.load(rule_directive);
    engine.init();
    auto t = engine.makeTransaction();
    ASSERT_TRUE(result.has_value());

    for (auto& rule : engine.rules(1)) {
      for (auto& action : rule.actions()) {
        action->evaluate(*t);
      }
    }
    EXPECT_EQ(std::get<std::string_view>(t->getVariable("", "foo")), "bar");
    int64_t score = std::get<int64_t>(t->getVariable("", "barscore"));
    EXPECT_EQ(score, 1);
  }

  // Create and init
  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,setvar:'tx.score2=100',msg:'aaa'")";

    Engine engine(spdlog::level::off);
    auto result = engine.load(rule_directive);
    engine.init();
    auto t = engine.makeTransaction();
    ASSERT_TRUE(result.has_value());

    auto& actions = engine.rules(1).back().actions();
    EXPECT_EQ(actions.size(), 1);
    actions.back()->evaluate(*t);
    int64_t score = std::get<int64_t>(t->getVariable("", "score2"));
    EXPECT_EQ(score, 100);
  }

  // Create and init (Macro expansion)
  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,setvar:'tx.foo=bar',msg:'aaa'"
        SecRule ARGS:aaa|ARGS:bbb "bar" "id:2,phase:1,setvar:'tx.score2=100',msg:'aaa'"
        SecRule ARGS:aaa|ARGS:bbb "bar" "id:3,phase:1,setvar:'tx.score_%{tx.foo}=%{tx.score2}',msg:'aaa'")";

    Engine engine(spdlog::level::off);
    auto result = engine.load(rule_directive);
    engine.init();
    auto t = engine.makeTransaction();
    ASSERT_TRUE(result.has_value());

    for (auto& rule : engine.rules(1)) {
      for (auto& action : rule.actions()) {
        action->evaluate(*t);
      }
    }
    int64_t score2 = std::get<int64_t>(t->getVariable("", "score2"));
    int64_t score_bar = std::get<int64_t>(t->getVariable("", "score_bar"));
    EXPECT_EQ(score2, score_bar);
  }

  // Create and init (Multi macro expansion)
  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,setvar:'tx.score',setvar:'tx.score2=100',msg:'aaa'"
        SecRule ARGS:aaa|ARGS:bbb "bar" "id:2,phase:1,setvar:'tx.foo2=%{tx.score2}_%{tx.score}',msg:'aaa'")";

    Engine engine(spdlog::level::off);
    auto result = engine.load(rule_directive);
    engine.init();
    auto t = engine.makeTransaction();
    ASSERT_TRUE(result.has_value());

    for (auto& rule : engine.rules(1)) {
      for (auto& action : rule.actions()) {
        action->evaluate(*t);
      }
    }
    int64_t score2 = std::get<int64_t>(t->getVariable("", "score2"));
    int64_t score = std::get<int64_t>(t->getVariable("", "score"));
    auto foo = std::get<std::string_view>(t->getVariable("", "foo2"));
    EXPECT_EQ(foo, std::format("{}_{}", score2, score));
  }

  // Remove
  {
    const std::string rule_directive1 =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,setvar:'tx.score2',msg:'aaa'")";

    const std::string rule_directive2 =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:2,phase:1,setvar:'!tx.score2',msg:'aaa'")";

    Engine engine(spdlog::level::off);
    auto result = engine.load(rule_directive1);
    engine.init();
    auto t = engine.makeTransaction();
    ASSERT_TRUE(result.has_value());

    auto& actions1 = engine.rules(1).back().actions();
    EXPECT_EQ(actions1.size(), 1);
    actions1.back()->evaluate(*t);
    EXPECT_EQ(std::get<int64_t>(t->getVariable("", "score2")), 1);

    result = engine.load(rule_directive2);
    engine.init();
    auto& actions2 = engine.rules(1).back().actions();
    EXPECT_EQ(actions2.size(), 1);
    actions2.back()->evaluate(*t);
    EXPECT_FALSE(t->hasVariable("", "score2"));
  }

  // Remove (Macro expansion)
  {
    const std::string rule_directive1 =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,setvar:'tx.foo=bar',setvar:'tx.score_bar',msg:'aaa'")";

    const std::string rule_directive2 =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:2,phase:1,setvar:'!tx.score_%{tx.foo}',msg:'aaa'")";

    Engine engine(spdlog::level::off);
    auto result = engine.load(rule_directive1);
    engine.init();
    auto t = engine.makeTransaction();
    ASSERT_TRUE(result.has_value());

    auto& actions1 = engine.rules(1).back().actions();
    for (auto& action : actions1) {
      action->evaluate(*t);
    }
    EXPECT_EQ(std::get<std::string_view>(t->getVariable("", "foo")), "bar");
    EXPECT_EQ(std::get<int64_t>(t->getVariable("", "score_bar")), 1);

    result = engine.load(rule_directive2);
    engine.init();
    auto& actions2 = engine.rules(1).back().actions();
    for (auto& action : actions2) {
      action->evaluate(*t);
    }
    EXPECT_FALSE(t->hasVariable("", "score_bar"));
  }

  // Increase
  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,setvar:'tx.score1=100',msg:'aaa'"
        SecRule ARGS:aaa|ARGS:bbb "bar" "id:2,phase:1,setvar:'tx.score1=+100',msg:'aaa'")";

    Engine engine(spdlog::level::off);
    auto result = engine.load(rule_directive);
    engine.init();
    auto t = engine.makeTransaction();
    ASSERT_TRUE(result.has_value());

    for (auto& rule : engine.rules(1)) {
      auto& actions = rule.actions();
      EXPECT_EQ(actions.size(), 1);
      actions.back()->evaluate(*t);
    }
    EXPECT_EQ(std::get<int64_t>(t->getVariable("", "score1")), 200);
  }

  // Increase (value macro expansion)
  {
    const std::string rule_directive1 =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,setvar:'tx.score200=200',setvar:'tx.score=100',msg:'aaa'")";

    const std::string rule_directive2 =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:2,phase:1,setvar:'tx.score%{tx.score200}=+%{tx.score}',msg:'aaa'")";

    Engine engine(spdlog::level::off);
    auto result = engine.load(rule_directive1);
    engine.init();
    auto t = engine.makeTransaction();
    ASSERT_TRUE(result.has_value());

    auto& actions1 = engine.rules(1).back().actions();
    for (auto& action : actions1) {
      action->evaluate(*t);
    }
    EXPECT_EQ(std::get<int64_t>(t->getVariable("", "score200")), 200);
    EXPECT_EQ(std::get<int64_t>(t->getVariable("", "score")), 100);

    result = engine.load(rule_directive2);
    engine.init();
    ASSERT_TRUE(result.has_value());

    auto& actions2 = engine.rules(1).back().actions();
    for (auto& action : actions2) {
      action->evaluate(*t);
    }
    EXPECT_EQ(std::get<int64_t>(t->getVariable("", "score200")), 300);
    EXPECT_EQ(std::get<int64_t>(t->getVariable("", "score")), 100);
  }

  // Increase (value macro expansion but not a integer)
  {
    const std::string rule_directive1 =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,setvar:'tx.score200=200',setvar:'tx.score=hello',msg:'aaa'")";

    const std::string rule_directive2 =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:2,phase:1,setvar:'tx.score%{tx.score200}=+%{tx.score}',msg:'aaa'")";

    const std::string rule_directive3 =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:3,phase:1,setvar:'tx.score%{tx.score200}=+hi',msg:'aaa'")";

    Engine engine(spdlog::level::off);
    auto result = engine.load(rule_directive1);
    engine.init();
    auto t = engine.makeTransaction();
    ASSERT_TRUE(result.has_value());

    auto& actions1 = engine.rules(1).back().actions();
    for (auto& action : actions1) {
      action->evaluate(*t);
    }
    EXPECT_EQ(std::get<int64_t>(t->getVariable("", "score200")), 200);
    EXPECT_EQ(std::get<std::string_view>(t->getVariable("", "score")), "hello");

    result = engine.load(rule_directive2);
    engine.init();
    ASSERT_TRUE(result.has_value());

    auto& actions2 = engine.rules(1).back().actions();
    for (auto& action : actions2) {
      action->evaluate(*t);
    }
    EXPECT_EQ(std::get<int64_t>(t->getVariable("", "score200")), 200);
    EXPECT_EQ(std::get<std::string_view>(t->getVariable("", "score")), "hello");

    result = engine.load(rule_directive3);
    engine.init();
    ASSERT_TRUE(result.has_value());

    auto& actions3 = engine.rules(1).back().actions();
    for (auto& action : actions3) {
      action->evaluate(*t);
    }
    EXPECT_EQ(std::get<int64_t>(t->getVariable("", "score200")), 200);
  }

  // Decrease
  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,setvar:'tx.score1=100',msg:'aaa'"
    SecRule ARGS:aaa|ARGS:bbb "bar" "id:2,phase:1,setvar:'tx.score1=-50',msg:'aaa'")";

    Engine engine(spdlog::level::off);
    auto result = engine.load(rule_directive);
    engine.init();
    auto t = engine.makeTransaction();
    ASSERT_TRUE(result.has_value());

    for (auto& rule : engine.rules(1)) {
      auto& actions = rule.actions();
      EXPECT_EQ(actions.size(), 1);
      actions.back()->evaluate(*t);
    }
    EXPECT_EQ(std::get<int64_t>(t->getVariable("", "score1")), 50);
  }

  // Decrease (value macro expansion)
  {
    const std::string rule_directive1 =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,setvar:'tx.score200=200',setvar:'tx.score=100',msg:'aaa'")";

    const std::string rule_directive2 =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:2,phase:1,setvar:'tx.score%{tx.score200}=-%{tx.score}',msg:'aaa'")";

    Engine engine(spdlog::level::off);
    auto result = engine.load(rule_directive1);
    engine.init();
    auto t = engine.makeTransaction();
    ASSERT_TRUE(result.has_value());

    auto& actions1 = engine.rules(1).back().actions();
    for (auto& action : actions1) {
      action->evaluate(*t);
    }
    EXPECT_EQ(std::get<int64_t>(t->getVariable("", "score200")), 200);
    EXPECT_EQ(std::get<int64_t>(t->getVariable("", "score")), 100);

    result = engine.load(rule_directive2);
    engine.init();
    ASSERT_TRUE(result.has_value());

    auto& actions2 = engine.rules(1).back().actions();
    for (auto& action : actions2) {
      action->evaluate(*t);
    }
    EXPECT_EQ(std::get<int64_t>(t->getVariable("", "score200")), 100);
    EXPECT_EQ(std::get<int64_t>(t->getVariable("", "score")), 100);
  }

  // Decrease (value macro expansion but not a integer)
  {
    const std::string rule_directive1 =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,setvar:'tx.score200=200',setvar:'tx.score=hello',msg:'aaa'")";

    const std::string rule_directive2 =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:2,phase:1,setvar:'tx.score%{tx.score200}=-%{tx.score}',msg:'aaa'")";

    const std::string rule_directive3 =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:3,phase:1,setvar:'tx.score%{tx.score200}=-hi',msg:'aaa'")";

    Engine engine(spdlog::level::off);
    auto result = engine.load(rule_directive1);
    engine.init();
    auto t = engine.makeTransaction();
    ASSERT_TRUE(result.has_value());

    auto& actions1 = engine.rules(1).back().actions();
    for (auto& action : actions1) {
      action->evaluate(*t);
    }
    EXPECT_EQ(std::get<int64_t>(t->getVariable("", "score200")), 200);
    EXPECT_EQ(std::get<std::string_view>(t->getVariable("", "score")), "hello");

    result = engine.load(rule_directive2);
    engine.init();
    ASSERT_TRUE(result.has_value());

    auto& actions2 = engine.rules(1).back().actions();
    for (auto& action : actions2) {
      action->evaluate(*t);
    }
    EXPECT_EQ(std::get<int64_t>(t->getVariable("", "score200")), 200);
    EXPECT_EQ(std::get<std::string_view>(t->getVariable("", "score")), "hello");

    result = engine.load(rule_directive3);
    engine.init();
    ASSERT_TRUE(result.has_value());

    auto& actions3 = engine.rules(1).back().actions();
    for (auto& action : actions3) {
      action->evaluate(*t);
    }
    EXPECT_EQ(std::get<int64_t>(t->getVariable("", "score200")), 200);
  }
}

TEST_F(RuleActionTest, ActionSetVarWithNoSigleQuote) {
  // Create
  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,setvar:tx.score,msg:'aaa'")";

    Engine engine(spdlog::level::off);
    auto result = engine.load(rule_directive);
    engine.init();
    auto t = engine.makeTransaction();
    ASSERT_TRUE(result.has_value());

    auto& actions = engine.rules(1).back().actions();
    EXPECT_EQ(actions.size(), 1);
    actions.back()->evaluate(*t);
    int64_t score = std::get<int64_t>(t->getVariable("", "score"));
    EXPECT_EQ(score, 1);
  }

  // Create (Macro expansion)
  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,setvar:tx.foo=bar,msg:'aaa'"
          SecRule ARGS:aaa|ARGS:bbb "bar" "id:2,phase:1,setvar:tx.%{tx.foo}score,msg:'aaa'")";

    Engine engine(spdlog::level::off);
    auto result = engine.load(rule_directive);
    engine.init();
    auto t = engine.makeTransaction();
    ASSERT_TRUE(result.has_value());

    for (auto& rule : engine.rules(1)) {
      for (auto& action : rule.actions()) {
        action->evaluate(*t);
      }
    }
    EXPECT_EQ(std::get<std::string_view>(t->getVariable("", "foo")), "bar");
    int64_t score = std::get<int64_t>(t->getVariable("", "barscore"));
    EXPECT_EQ(score, 1);
  }

  // Create and init
  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,setvar:tx.score2=100,msg:'aaa'")";

    Engine engine(spdlog::level::off);
    auto result = engine.load(rule_directive);
    engine.init();
    auto t = engine.makeTransaction();
    ASSERT_TRUE(result.has_value());

    auto& actions = engine.rules(1).back().actions();
    EXPECT_EQ(actions.size(), 1);
    actions.back()->evaluate(*t);
    int64_t score = std::get<int64_t>(t->getVariable("", "score2"));
    EXPECT_EQ(score, 100);
  }

  // Create and init (Macro expansion)
  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,setvar:tx.foo=bar,msg:'aaa'"
          SecRule ARGS:aaa|ARGS:bbb "bar" "id:2,phase:1,setvar:tx.score2=100,msg:'aaa'"
          SecRule ARGS:aaa|ARGS:bbb "bar" "id:3,phase:1,setvar:tx.score_%{tx.foo}=%{tx.score2},msg:'aaa'")";

    Engine engine(spdlog::level::off);
    auto result = engine.load(rule_directive);
    engine.init();
    auto t = engine.makeTransaction();
    ASSERT_TRUE(result.has_value());

    for (auto& rule : engine.rules(1)) {
      for (auto& action : rule.actions()) {
        action->evaluate(*t);
      }
    }
    int64_t score2 = std::get<int64_t>(t->getVariable("", "score2"));
    int64_t score_bar = std::get<int64_t>(t->getVariable("", "score_bar"));
    EXPECT_EQ(score2, score_bar);
  }

  // Create and init (Multi macro expansion)
  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,setvar:tx.score,setvar:tx.score2=100,msg:'aaa'"
          SecRule ARGS:aaa|ARGS:bbb "bar" "id:2,phase:1,setvar:tx.foo2=%{tx.score2}_%{tx.score},msg:'aaa'")";

    Engine engine(spdlog::level::off);
    auto result = engine.load(rule_directive);
    engine.init();
    auto t = engine.makeTransaction();
    ASSERT_TRUE(result.has_value());

    for (auto& rule : engine.rules(1)) {
      for (auto& action : rule.actions()) {
        action->evaluate(*t);
      }
    }
    int64_t score2 = std::get<int64_t>(t->getVariable("", "score2"));
    int64_t score = std::get<int64_t>(t->getVariable("", "score"));
    auto foo = std::get<std::string_view>(t->getVariable("", "foo2"));
    EXPECT_EQ(foo, std::format("{}_{}", score2, score));
  }

  // Remove
  {
    const std::string rule_directive1 =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,setvar:tx.score2,msg:'aaa'")";

    const std::string rule_directive2 =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:2,phase:1,setvar:!tx.score2,msg:'aaa'")";

    Engine engine(spdlog::level::off);
    auto result = engine.load(rule_directive1);
    engine.init();
    auto t = engine.makeTransaction();
    ASSERT_TRUE(result.has_value());

    auto& actions1 = engine.rules(1).back().actions();
    EXPECT_EQ(actions1.size(), 1);
    actions1.back()->evaluate(*t);
    EXPECT_EQ(std::get<int64_t>(t->getVariable("", "score2")), 1);

    result = engine.load(rule_directive2);
    engine.init();
    auto& actions2 = engine.rules(1).back().actions();
    EXPECT_EQ(actions2.size(), 1);
    actions2.back()->evaluate(*t);
    EXPECT_FALSE(t->hasVariable("", "score2"));
  }

  // Remove (Macro expansion)
  {
    const std::string rule_directive1 =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,setvar:'tx.foo=bar',setvar:tx.score_bar,msg:'aaa'")";

    const std::string rule_directive2 =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:2,phase:1,setvar:!tx.score_%{tx.foo},msg:'aaa'")";

    Engine engine(spdlog::level::off);
    auto result = engine.load(rule_directive1);
    engine.init();
    auto t = engine.makeTransaction();
    ASSERT_TRUE(result.has_value());

    auto& actions1 = engine.rules(1).back().actions();
    for (auto& action : actions1) {
      action->evaluate(*t);
    }
    EXPECT_EQ(std::get<std::string_view>(t->getVariable("", "foo")), "bar");
    EXPECT_EQ(std::get<int64_t>(t->getVariable("", "score_bar")), 1);

    result = engine.load(rule_directive2);
    engine.init();
    auto& actions2 = engine.rules(1).back().actions();
    for (auto& action : actions2) {
      action->evaluate(*t);
    }
    EXPECT_FALSE(t->hasVariable("", "score_bar"));
  }

  // Increase
  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,setvar:tx.score1=100,msg:'aaa'"
          SecRule ARGS:aaa|ARGS:bbb "bar" "id:2,phase:1,setvar:tx.score1=+100,msg:'aaa'")";

    Engine engine(spdlog::level::off);
    auto result = engine.load(rule_directive);
    engine.init();
    auto t = engine.makeTransaction();
    ASSERT_TRUE(result.has_value());

    for (auto& rule : engine.rules(1)) {
      auto& actions = rule.actions();
      EXPECT_EQ(actions.size(), 1);
      actions.back()->evaluate(*t);
    }
    EXPECT_EQ(std::get<int64_t>(t->getVariable("", "score1")), 200);
  }

  // Increase (value macro expansion)
  {
    const std::string rule_directive1 =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,setvar:tx.score200=200,setvar:'tx.score=100',msg:'aaa'")";

    const std::string rule_directive2 =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:2,phase:1,setvar:tx.score%{tx.score200}=+%{tx.score},msg:'aaa'")";

    Engine engine(spdlog::level::off);
    auto result = engine.load(rule_directive1);
    engine.init();
    auto t = engine.makeTransaction();
    ASSERT_TRUE(result.has_value());

    auto& actions1 = engine.rules(1).back().actions();
    for (auto& action : actions1) {
      action->evaluate(*t);
    }
    EXPECT_EQ(std::get<int64_t>(t->getVariable("", "score200")), 200);
    EXPECT_EQ(std::get<int64_t>(t->getVariable("", "score")), 100);

    result = engine.load(rule_directive2);
    engine.init();
    ASSERT_TRUE(result.has_value());

    auto& actions2 = engine.rules(1).back().actions();
    for (auto& action : actions2) {
      action->evaluate(*t);
    }
    EXPECT_EQ(std::get<int64_t>(t->getVariable("", "score200")), 300);
    EXPECT_EQ(std::get<int64_t>(t->getVariable("", "score")), 100);
  }

  // Decrease
  {
    const std::string rule_directive =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,setvar:tx.score1=100,msg:'aaa'"
      SecRule ARGS:aaa|ARGS:bbb "bar" "id:2,phase:1,setvar:tx.score1=-50,msg:'aaa'")";

    Engine engine(spdlog::level::off);
    auto result = engine.load(rule_directive);
    engine.init();
    auto t = engine.makeTransaction();
    ASSERT_TRUE(result.has_value());

    for (auto& rule : engine.rules(1)) {
      auto& actions = rule.actions();
      EXPECT_EQ(actions.size(), 1);
      actions.back()->evaluate(*t);
    }
    EXPECT_EQ(std::get<int64_t>(t->getVariable("", "score1")), 50);
  }

  // Decrease (value macro expansion)
  {
    const std::string rule_directive1 =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,setvar:tx.score200=200,setvar:'tx.score=100',msg:'aaa'")";

    const std::string rule_directive2 =
        R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:2,phase:1,setvar:tx.score%{tx.score200}=-%{tx.score},msg:'aaa'")";

    Engine engine(spdlog::level::off);
    auto result = engine.load(rule_directive1);
    engine.init();
    auto t = engine.makeTransaction();
    ASSERT_TRUE(result.has_value());

    auto& actions1 = engine.rules(1).back().actions();
    for (auto& action : actions1) {
      action->evaluate(*t);
    }
    EXPECT_EQ(std::get<int64_t>(t->getVariable("", "score200")), 200);
    EXPECT_EQ(std::get<int64_t>(t->getVariable("", "score")), 100);

    result = engine.load(rule_directive2);
    engine.init();
    ASSERT_TRUE(result.has_value());

    auto& actions2 = engine.rules(1).back().actions();
    for (auto& action : actions2) {
      action->evaluate(*t);
    }
    EXPECT_EQ(std::get<int64_t>(t->getVariable("", "score200")), 100);
    EXPECT_EQ(std::get<int64_t>(t->getVariable("", "score")), 100);
  }
}

TEST_F(RuleActionTest, ActionSetEnv) {
  const std::string rule_directive =
      R"(SecRule ARGS:aaa|ARGS:bbb "bar" "id:1,phase:1,setenv:'var1=hello',msg:'aaa bbb'")";

  Engine engine(spdlog::level::off);
  auto result = engine.load(rule_directive);
  engine.init();
  auto t = engine.makeTransaction();
  ASSERT_TRUE(result.has_value());

  auto& actions = engine.rules(1).back().actions();
  EXPECT_EQ(actions.size(), 1);
  actions.back()->evaluate(*t);
  EXPECT_EQ(std::string("hello"), ::getenv("var1"));
}

TEST_F(RuleActionTest, ActionAllow) {
  const std::string rule_directive =
      R"(SecRuleEngine On
      SecAction "phase:1,setvar:tx.test=1"
      SecRule TX:test "@eq 1" "allow,id:1,phase:1,setvar:tx.phase1=true,msg:'aaa bbb'"
      SecRule TX:test "@eq 1" "id:12,phase:1,setvar:tx.phase1_2=true,msg:'aaa bbb'"
      SecRule TX:test "@eq 1" "id:2,phase:2,setvar:tx.phase2=true,msg:'aaa bbb'"
      SecRule TX:test "@eq 1" "id:3,phase:3,setvar:tx.phase3=true,msg:'aaa bbb'"
      SecRule TX:test "@eq 1" "id:4,phase:4,setvar:tx.phase4=true,msg:'aaa bbb'")";

  Engine engine(spdlog::level::off);
  auto result = engine.load(rule_directive);
  ASSERT_TRUE(result.has_value());
  engine.init();
  auto t = engine.makeTransaction();

  t->processRequestHeaders(nullptr, nullptr, 0, nullptr);
  t->processRequestBody("", nullptr);
  t->processResponseHeaders("", "", nullptr, nullptr, 0, nullptr);
  t->processResponseBody("", nullptr);
  EXPECT_TRUE(t->hasVariable("", "phase1"));
  EXPECT_FALSE(t->hasVariable("", "phase1_2"));
  EXPECT_FALSE(t->hasVariable("", "phase2"));
  EXPECT_FALSE(t->hasVariable("", "phase3"));
  EXPECT_FALSE(t->hasVariable("", "phase4"));
}

TEST_F(RuleActionTest, ActionAllowPhase) {
  const std::string rule_directive =
      R"(SecRuleEngine On
      SecAction "phase:1,setvar:tx.test=1"
      SecRule TX:test "@eq 1" "allow:phase,id:1,phase:1,setvar:tx.phase1=true,msg:'aaa bbb'"
      SecRule TX:test "@eq 1" "id:12,phase:1,setvar:tx.phase1_2=true,msg:'aaa bbb'"
      SecRule TX:test "@eq 1" "id:2,phase:2,setvar:tx.phase2=true,msg:'aaa bbb'"
      SecRule TX:test "@eq 1" "id:3,phase:3,setvar:tx.phase3=true,msg:'aaa bbb'"
      SecRule TX:test "@eq 1" "id:4,phase:4,setvar:tx.phase4=true,msg:'aaa bbb'")";

  Engine engine(spdlog::level::off);
  auto result = engine.load(rule_directive);
  ASSERT_TRUE(result.has_value());
  engine.init();
  auto t = engine.makeTransaction();

  t->processRequestHeaders(nullptr, nullptr, 0, nullptr);
  t->processRequestBody("", nullptr);
  t->processResponseHeaders("", "", nullptr, nullptr, 0, nullptr);
  t->processResponseBody("", nullptr);
  EXPECT_TRUE(t->hasVariable("", "phase1"));
  EXPECT_FALSE(t->hasVariable("", "phase1_2"));
  EXPECT_TRUE(t->hasVariable("", "phase2"));
  EXPECT_TRUE(t->hasVariable("", "phase3"));
  EXPECT_TRUE(t->hasVariable("", "phase4"));
}

TEST_F(RuleActionTest, ActionAllowRequest) {
  const std::string rule_directive =
      R"(SecRuleEngine On
      SecAction "phase:1,setvar:tx.test=1"
      SecRule TX:test "@eq 1" "allow:request,id:1,phase:1,setvar:tx.phase1=true,msg:'aaa bbb'"
      SecRule TX:test "@eq 1" "id:12,phase:1,setvar:tx.phase1_2=true,msg:'aaa bbb'"
      SecRule TX:test "@eq 1" "id:2,phase:2,setvar:tx.phase2=true,msg:'aaa bbb'"
      SecRule TX:test "@eq 1" "id:3,phase:3,setvar:tx.phase3=true,msg:'aaa bbb'"
      SecRule TX:test "@eq 1" "id:4,phase:4,setvar:tx.phase4=true,msg:'aaa bbb'")";

  Engine engine(spdlog::level::off);
  auto result = engine.load(rule_directive);
  ASSERT_TRUE(result.has_value());
  engine.init();
  auto t = engine.makeTransaction();

  t->processRequestHeaders(nullptr, nullptr, 0, nullptr);
  t->processRequestBody("", nullptr);
  t->processResponseHeaders("", "", nullptr, nullptr, 0, nullptr);
  t->processResponseBody("", nullptr);
  EXPECT_TRUE(t->hasVariable("", "phase1"));
  EXPECT_FALSE(t->hasVariable("", "phase1_2"));
  EXPECT_FALSE(t->hasVariable("", "phase2"));
  EXPECT_TRUE(t->hasVariable("", "phase3"));
  EXPECT_TRUE(t->hasVariable("", "phase4"));
}

TEST_F(RuleActionTest, ActionFirstMatch) {
  {
    const std::string rule_directive =
        R"(SecRuleEngine On
      SecAction "phase:1,setvar:tx.test1=1,setvar:tx.test2=1,setvar:tx.test3=1"
      SecRule TX "@gt 0" "allow:request,id:1,phase:1,setvar:tx.result=+1,msg:'aaa bbb'")";

    Engine engine(spdlog::level::off);
    auto result = engine.load(rule_directive);
    ASSERT_TRUE(result.has_value());
    engine.init();
    auto t = engine.makeTransaction();
    t->processRequestHeaders(nullptr, nullptr, 0, nullptr);
    EXPECT_EQ(std::get<int64_t>(t->getVariable("", "result")), 3);
  }

  {
    const std::string rule_directive =
        R"(SecRuleEngine On
      SecAction "phase:1,setvar:tx.test1=1,setvar:tx.test2=1,setvar:tx.test3=1"
      SecRule TX "@gt 0" "allow:request,id:1,phase:1,setvar:tx.result=+1,firstMatch,msg:'aaa bbb'")";

    Engine engine(spdlog::level::off);
    auto result = engine.load(rule_directive);
    ASSERT_TRUE(result.has_value());
    engine.init();
    auto t = engine.makeTransaction();
    t->processRequestHeaders(nullptr, nullptr, 0, nullptr);
    EXPECT_EQ(std::get<int64_t>(t->getVariable("", "result")), 1);
  }
}

TEST_F(RuleActionTest, ActionEmptyMatch) {
  {
    const std::string rule_directive =
        R"(SecRuleEngine On
      SecAction "phase:1,setvar:tx.aaa=1"
      SecRule TX:aaa "@gt %{tx.test}" "allow:request,id:1,phase:1,setvar:tx.result=+1,msg:'aaa bbb'")";

    Engine engine(spdlog::level::off);
    auto result = engine.load(rule_directive);
    ASSERT_TRUE(result.has_value());
    engine.init();
    auto t = engine.makeTransaction();
    t->processRequestHeaders(nullptr, nullptr, 0, nullptr);
    EXPECT_FALSE(t->hasVariable("", "result"));
  }

  {
    const std::string rule_directive =
        R"(SecRuleEngine On
      SecAction "phase:1,setvar:tx.aaa=1"
      SecRule TX:aaa "@gt %{tx.test}" "allow:request,id:1,phase:1,setvar:tx.result=+1,emptyMatch,msg:'aaa bbb'")";

    Engine engine(spdlog::level::trace);
    auto result = engine.load(rule_directive);
    ASSERT_TRUE(result.has_value());
    engine.init();
    auto t = engine.makeTransaction();
    t->processRequestHeaders(nullptr, nullptr, 0, nullptr);
    EXPECT_EQ(std::get<int64_t>(t->getVariable("", "result")), 1);
  }
}

TEST_F(RuleActionTest, ActionAllMatch) {
  {
    const std::string rule_directive =
        R"(SecRuleEngine On
      SecAction "phase:1,setvar:tx.foo=100,setvar:tx.bar=200"
      SecRule TX "@gt 100" "allow:request,id:1,phase:1,setvar:tx.result=+1,msg:'aaa bbb'")";

    Engine engine(spdlog::level::off);
    auto result = engine.load(rule_directive);
    ASSERT_TRUE(result.has_value());
    engine.init();
    auto t = engine.makeTransaction();
    t->processRequestHeaders(nullptr, nullptr, 0, nullptr);
    EXPECT_EQ(std::get<int64_t>(t->getVariable("", "result")), 1);
  }

  {
    const std::string rule_directive =
        R"(SecRuleEngine On
      SecAction "phase:1,setvar:tx.foo=100,setvar:tx.bar=200"
      SecRule TX "@gt 100" "allow:request,id:1,phase:1,allMatch,setvar:tx.result=+1,msg:'aaa bbb'")";

    Engine engine(spdlog::level::off);
    auto result = engine.load(rule_directive);
    ASSERT_TRUE(result.has_value());
    engine.init();
    auto t = engine.makeTransaction();
    t->processRequestHeaders(nullptr, nullptr, 0, nullptr);
    EXPECT_FALSE(t->hasVariable("", "result"));
  }
}

} // namespace Integration
} // namespace Wge