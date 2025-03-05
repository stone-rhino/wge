#include <gtest/gtest.h>

#include "action/set_var.h"
#include "engine.h"
#include "macro/macro_include.h"
#include "variable/variables_include.h"

namespace SrSecurity {
class ActionEvaluate : public testing::Test {
public:
  ActionEvaluate() : engine_(Engine::singleton()) {}

public:
  Engine& engine_;
};

TEST_F(ActionEvaluate, SetVar) {
  auto t = engine_.makeTransaction();
  Action::SetVar set_var("score", Common::Variant(), Action::SetVar::EvaluateType::Create);
  set_var.evaluate(*t);
  int score = std::get<int>(t->getVariable("score"));
  EXPECT_EQ(score, 1);

  {
    Action::SetVar set_var("score", 100, Action::SetVar::EvaluateType::Increase);
    set_var.evaluate(*t);
    int score = std::get<int>(t->getVariable("score"));
    EXPECT_EQ(score, 101);
  }

  {
    Action::SetVar set_var("score", 50, Action::SetVar::EvaluateType::Decrease);
    set_var.evaluate(*t);
    int score = std::get<int>(t->getVariable("score"));
    EXPECT_EQ(score, 51);
  }

  {
    Action::SetVar set_var("score2", 100, Action::SetVar::EvaluateType::CreateAndInit);
    set_var.evaluate(*t);
    int score = std::get<int>(t->getVariable("score2"));
    EXPECT_EQ(score, 100);
  }

  {
    Action::SetVar set_var("score2", Common::Variant(), Action::SetVar::EvaluateType::Remove);
    set_var.evaluate(*t);
    EXPECT_TRUE(IS_EMPTY_VARIANT(t->getVariable("score2")));
  }
}

TEST_F(ActionEvaluate, SetVarMacroTx) {
  auto t = engine_.makeTransaction();
  Action::SetVar set_var("score", 100, Action::SetVar::EvaluateType::CreateAndInit);
  set_var.evaluate(*t);
  int score = std::get<int>(t->getVariable("score"));
  EXPECT_EQ(score, 100);

  {
    std::shared_ptr<Variable::VariableBase> var =
        std::make_shared<Variable::Tx>("score", false, false);
    Action::SetVar set_var("score2", std::make_shared<Macro::VariableMacro>(var),
                           Action::SetVar::EvaluateType::CreateAndInit);
    set_var.evaluate(*t);
    int score = std::get<int>(t->getVariable("score2"));
    EXPECT_EQ(score, 100);
  }
}
} // namespace SrSecurity