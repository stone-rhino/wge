#include <gtest/gtest.h>

#include "action/set_var.h"
#include "engine.h"

namespace SrSecurity {
class ActionEvaluate : public testing::Test {
public:
  Engine engine_;
};

TEST_F(ActionEvaluate, SetVar) {
  auto t = engine_.makeTransaction();
  Action::SetVar set_var("tx.score");
  set_var.evaluate(t.get());
  int score = t->getVariableInt("score");
  EXPECT_EQ(score, 1);

  {
    Action::SetVar set_var("tx.score=+100");
    set_var.evaluate(t.get());
    int score = t->getVariableInt("score");
    EXPECT_EQ(score, 101);
  }

  {
    Action::SetVar set_var("tx.score=-50");
    set_var.evaluate(t.get());
    int score = t->getVariableInt("score");
    EXPECT_EQ(score, 51);
  }

  {
    Action::SetVar set_var("tx.score2=100");
    set_var.evaluate(t.get());
    int score = t->getVariableInt("score2");
    EXPECT_EQ(score, 100);
  }

  {
    Action::SetVar set_var("!tx.score2");
    set_var.evaluate(t.get());
    int score = t->getVariableInt("score2");
    EXPECT_EQ(score, 0);
  }
}

TEST_F(ActionEvaluate, SetVarMacroTx) {
  auto t = engine_.makeTransaction();
  Action::SetVar set_var("tx.score=100");
  set_var.evaluate(t.get());
  int score = t->getVariableInt("score");
  EXPECT_EQ(score, 100);

  {
    Action::SetVar set_var("tx.score2=%{tx.score}");
    set_var.evaluate(t.get());
    int score = t->getVariableInt("score2");
    EXPECT_EQ(score, 100);
  }
}
} // namespace SrSecurity