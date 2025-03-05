#include <string>

#include <gtest/gtest.h>

#include "antlr4/parser.h"
#include "engine.h"

namespace SrSecurity {
namespace Parsr {
class EngineActionTest : public testing::Test {
public:
  EngineActionTest() : engine_(spdlog::level::trace) {}

protected:
  Engine engine_;
};

TEST_F(EngineActionTest, SecAction) {
  auto t = engine_.makeTransaction();

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

  auto& actions = rule->actions();
  for (auto& action : actions) {
    action->evaluate(*t);
  }
  int score = std::get<int>(t->getVariable("score"));
  int score1 = std::get<int>(t->getVariable("score1"));
  EXPECT_EQ(score, 100);
  EXPECT_EQ(score1, 100);
}

TEST_F(EngineActionTest, SecDefaultAction) {
  auto t = engine_.makeTransaction();

  const std::string directive =
      R"(SecDefaultAction "phase:1,log,auditlog,pass"
      SecDefaultAction "phase:2,log,auditlog,pass")";

  Antlr4::Parser parser;
  auto result = parser.load(directive);
  ASSERT_TRUE(result.has_value());
  auto& rules = parser.defaultActions();
  EXPECT_EQ(rules.size(), 2);
  auto& rule = rules.front();
  EXPECT_EQ(rule->phase(), 1);
  EXPECT_TRUE(rule->log().value_or(false));
  EXPECT_TRUE(rule->auditLog().value_or(false));
  EXPECT_EQ(rule->disruptive(), Rule::Disruptive::PASS);
}
} // namespace Parsr
} // namespace SrSecurity