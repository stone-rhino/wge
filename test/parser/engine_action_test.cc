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
#include <string>

#include <gtest/gtest.h>

#include "antlr4/parser.h"
#include "engine.h"

namespace SrSecurity {
namespace Parsr {
class EngineActionTest : public testing::Test {
private:
  // Use for specific the main thread id, so that the ASSERT_IS_MAIN_THREAD macro can work
  // correctly in the test.
  Engine main_thread_id_init_helper_;
};

TEST_F(EngineActionTest, SecAction) {
  const std::string directive =
      R"(SecAction "id:1,phase:2,setvar:'tx.score=100',setvar:'tx.score1=%{tx.score}'")";

  Antlr4::Parser parser;
  auto result = parser.load(directive);
  ASSERT_TRUE(result.has_value());

  auto& rules = parser.rules();
  EXPECT_EQ(rules.size(), 1);
  auto& rule = rules.front();
  EXPECT_EQ(rule->id(), 1);
  EXPECT_EQ(rule->phase(), 2);
  EXPECT_EQ(rule->getOperator(), nullptr);

  auto& actions = rule->actions();
  EXPECT_EQ(actions.size(), 2);
  EXPECT_EQ(actions[0]->name(), std::string_view("setvar"));
  EXPECT_EQ(actions[1]->name(), std::string_view("setvar"));
}

TEST_F(EngineActionTest, SecDefaultAction) {
  const std::string directive =
      R"(SecDefaultAction "phase:1,log,auditlog,pass"
      SecDefaultAction "phase:2,log,auditlog,pass")";

  Antlr4::Parser parser;
  auto result = parser.load(directive);
  ASSERT_TRUE(result.has_value());

  auto& rules = parser.defaultActions();
  EXPECT_EQ(rules.size(), 2);
  auto& rule1 = rules.front();
  auto& rule2 = rules.back();
  EXPECT_EQ(rule1->phase(), 1);
  EXPECT_EQ(rule2->phase(), 2);
  EXPECT_TRUE(rule1->log().value_or(false));
  EXPECT_TRUE(rule1->auditLog().value_or(false));
  EXPECT_EQ(rule1->disruptive(), Rule::Disruptive::PASS);
  EXPECT_TRUE(rule2->log().value_or(false));
  EXPECT_TRUE(rule2->auditLog().value_or(false));
  EXPECT_EQ(rule2->disruptive(), Rule::Disruptive::PASS);
}
} // namespace Parsr
} // namespace SrSecurity