#pragma once

#include "operator_base.h"

namespace SrSecurity {
namespace Operator {
/**
 * Performs a string comparison and returns true if the parameter string is identical to the input
 * string. Macro expansion is performed on the parameter string before comparison.
 */
class Streq : public OperatorBase {
  DECLARE_OPERATOR_NAME(streq);

public:
  Streq(std::string&& literal_value, bool is_not, std::string_view curr_rule_file_path)
      : OperatorBase(std::move(literal_value), is_not) {}

  Streq(const std::shared_ptr<Macro::MacroBase> macro, bool is_not,
        std::string_view curr_rule_file_path)
      : OperatorBase(macro, is_not) {}

public:
  bool evaluate(Transaction& t, const Common::Variant& operand) const override {
    if (IS_STRING_VIEW_VARIANT(operand)) [[likely]] {
      if (!macro_) [[likely]] {
        return is_not_ ^ (literal_value_ == std::get<std::string_view>(operand));
      } else {
        MACRO_EXPAND_STRING_VIEW(macro_value);
        return is_not_ ^ (macro_value == std::get<std::string_view>(operand));
      }
    } else {
      return false;
    }
  }
};
} // namespace Operator
} // namespace SrSecurity