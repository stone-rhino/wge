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
  const int& score = t->getVariable<int>("score", 0);
  EXPECT_EQ(score, 1);

  {
    Action::SetVar set_var("tx.score+=100");
    set_var.evaluate(t.get());
    const int& score = t->getVariable<int>("score", 0);
    EXPECT_EQ(score, 101);
  }

  {
    Action::SetVar set_var("tx.score2=100");
    set_var.evaluate(t.get());
    const int& score = t->getVariable<int>("score2", 0);
    EXPECT_EQ(score, 100);
  }

  {
    Action::SetVar set_var("!tx.score2");
    set_var.evaluate(t.get());
    const int& score = t->getVariable<int>("score2", 0);
    EXPECT_EQ(score, 0);
  }
}
} // namespace SrSecurity