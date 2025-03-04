#pragma once

#include "macro_base.h"
#include "memory"

#include "../common/log.h"
#include "../variable/variable_base.h"

namespace SrSecurity {
namespace Macro {
class VariableMacro : public MacroBase {
public:
  VariableMacro(const std::shared_ptr<Variable::VariableBase> variable) : variable_(variable) {}

public:
  const Common::Variant& evaluate(Transaction& t) override {
    SRSECURITY_LOG_TRACE("macro %{{{}}} expanded: {}", makeVariableName(),
                         VISTIT_VARIANT_AS_STRING(variable_->evaluate(t)));
    return variable_->evaluate(t);
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