#include <string>

#include <gtest/gtest.h>

#include "antlr4/parser.h"
#include "engine.h"

namespace SrSecurity {
namespace Parsr {
class EngineActionTest : public testing::Test {
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
  int score = t->getVariableInt("score");
  int score1 = t->getVariableInt("score1");
  EXPECT_EQ(score, 100);
  EXPECT_EQ(score1, 100);
}
} // namespace Parsr
} // namespace SrSecurity