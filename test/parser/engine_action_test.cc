#include <string>

#include <gtest/gtest.h>

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
  const std::string directive =
      R"(SecAction "id:1,phase:2,setvar:'tx.score=100',setvar:'tx.score1=%{tx.score}'")";

  auto result = engine_.load(directive);
  engine_.init();
  auto t = engine_.makeTransaction();
  ASSERT_TRUE(result.has_value());

  auto& rules = engine_.rules(2);
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
  const std::string directive =
      R"(SecDefaultAction "phase:1,log,auditlog,pass"
      SecDefaultAction "phase:2,log,auditlog,pass")";

  auto result = engine_.load(directive);
  engine_.init();
  auto t = engine_.makeTransaction();
  ASSERT_TRUE(result.has_value());

  auto rule1 = engine_.defaultActions(1);
  auto rule2 = engine_.defaultActions(2);
  EXPECT_NE(rule1, nullptr);
  EXPECT_NE(rule2, nullptr);
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