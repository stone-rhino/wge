#include <gtest/gtest.h>

#include "action/set_var.h"
#include "engine.h"
#include "macro/tx.h"

namespace SrSecurity {
class ActionEvaluate : public testing::Test {
public:
  Engine engine_;
};

TEST_F(ActionEvaluate, SetVar) {
  auto t = engine_.makeTransaction();
  Action::SetVar set_var("score", "", Action::SetVar::EvaluateType::Create);
  set_var.evaluate(*t);
  int score = t->getVariableInt("score");
  EXPECT_EQ(score, 1);

  {
    Action::SetVar set_var("score", "100", Action::SetVar::EvaluateType::Increase);
    set_var.evaluate(*t);
    int score = t->getVariableInt("score");
    EXPECT_EQ(score, 101);
  }

  {
    Action::SetVar set_var("score", "50", Action::SetVar::EvaluateType::Decrease);
    set_var.evaluate(*t);
    int score = t->getVariableInt("score");
    EXPECT_EQ(score, 51);
  }

  {
    Action::SetVar set_var("score2", "100", Action::SetVar::EvaluateType::CreateAndInit);
    set_var.evaluate(*t);
    int score = t->getVariableInt("score2");
    EXPECT_EQ(score, 100);
  }

  {
    Action::SetVar set_var("score2", "", Action::SetVar::EvaluateType::Remove);
    set_var.evaluate(*t);
    int score = t->getVariableInt("score2");
    EXPECT_EQ(score, 0);
  }
}

TEST_F(ActionEvaluate, SetVarMacroTx) {
  auto t = engine_.makeTransaction();
  Action::SetVar set_var("score", "100", Action::SetVar::EvaluateType::CreateAndInit);
  set_var.evaluate(*t);
  int score = t->getVariableInt("score");
  EXPECT_EQ(score, 100);

  {
    Action::SetVar set_var("score2", std::make_shared<Macro::Tx>("score"),
                           Action::SetVar::EvaluateType::CreateAndInit);
    set_var.evaluate(*t);
    int score = t->getVariableInt("score2");
    EXPECT_EQ(score, 100);
  }
}
} // namespace SrSecurity