#pragma once

#include "macro_base.h"
#include "memory"

#include "../common/log.h"
#include "../variable/variable_base.h"

namespace SrSecurity {
namespace Macro {
class VariableMacro : public MacroBase {
public:
  VariableMacro(std::string&& literal_value, const std::shared_ptr<Variable::VariableBase> variable)
      : MacroBase(std::move(literal_value)), variable_(variable) {}

public:
  void evaluate(Transaction& t, Common::EvaluateResult& result) override {
    variable_->evaluate(t, result);
    SRSECURITY_LOG_TRACE("macro %{{{}}} expanded: {}", makeVariableName(),
                         VISTIT_VARIANT_AS_STRING(result.front()));
  }

private:
  std::string makeVariableName() {
    std::string name = variable_->mainName();
    if (!variable_->subName().empty()) {
      name += "." + variable_->subName();
    }
    return name;
  }

private:
  const std::shared_ptr<Variable::VariableBase> variable_;
};
} // namespace Macro
} // namespace SrSecurity