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
  const Common::Variant& evaluate(Transaction& t) override {
    // Save the old variable buffer to restore it after the variable is evaluated to avoid
    // overwriting the variable buffer.
    SrSecurity::Transaction::EvaluatedBuffer old_variable_buffer =
        std::move(t.getEvaluatedBuffer(Transaction::EvaluatedBufferType::Variable));

    SRSECURITY_LOG_TRACE("macro %{{{}}} expanded: {}", makeVariableName(),
                         VISTIT_VARIANT_AS_STRING(variable_->evaluate(t)));

    auto& result = variable_->evaluate(t);

    // Restore the old variable buffer.
    t.getEvaluatedBuffer(Transaction::EvaluatedBufferType::Variable) =
        std::move(old_variable_buffer);

    if (IS_STRING_VIEW_VARIANT(result)) {
      return t.getEvaluatedBuffer(Transaction::EvaluatedBufferType::Macro)
          .set(std::get<std::string_view>(result));
    } else if (IS_INT_VARIANT(result)) {
      return t.getEvaluatedBuffer(Transaction::EvaluatedBufferType::Macro)
          .set(std::get<int>(result));
    } else {
      return t.getEvaluatedBuffer(Transaction::EvaluatedBufferType::Macro).set();
    }
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