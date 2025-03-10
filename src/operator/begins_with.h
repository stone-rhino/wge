#pragma once

#include "operator_base.h"

namespace SrSecurity {
namespace Operator {
/**
 * Returns true if the parameter string is found at the beginning of the input. Macro expansion is
 * performed on the parameter string before comparison.
 */
class BeginsWith : public OperatorBase {
  DECLARE_OPERATOR_NAME(beginsWith);

public:
  BeginsWith(std::string&& literal_value, bool is_not)
      : OperatorBase(std::move(literal_value), is_not) {}

  BeginsWith(const std::shared_ptr<Macro::MacroBase> macro, bool is_not)
      : OperatorBase(macro, is_not) {}

public:
  bool evaluate(Transaction& t, const Common::Variant& operand) const override {
    if (IS_STRING_VIEW_VARIANT(operand)) [[likely]] {
      if (macro_) {
        Common::EvaluateResult result;
        macro_->evaluate(t, result);
        std::string_view macro_value = std::get<std::string_view>(result.get());
        return is_not_ ^ (std::get<std::string_view>(operand).starts_with(macro_value));
      } else {
        return is_not_ ^ (std::get<std::string_view>(operand).starts_with(literal_value_));
      }
    } else {
      return false;
    }
  }
};
} // namespace Operator
} // namespace SrSecurity