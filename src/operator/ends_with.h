#pragma once

#include "operator_base.h"

namespace SrSecurity {
namespace Operator {
class EndsWith : public OperatorBase {
  DECLARE_OPERATOR_NAME(endsWith);

public:
  EndsWith(std::string&& literal_value, bool is_not, std::string_view curr_rule_file_path)
      : OperatorBase(std::move(literal_value), is_not) {}

  EndsWith(const std::shared_ptr<Macro::MacroBase> macro, bool is_not,
           std::string_view curr_rule_file_path)
      : OperatorBase(macro, is_not) {}

public:
  bool evaluate(Transaction& t, const Common::Variant& operand) const override {
    if (IS_STRING_VIEW_VARIANT(operand)) [[likely]] {
      if (!macro_) [[likely]] {
        return is_not_ ^ (std::get<std::string_view>(operand).ends_with(literal_value_));
      } else {
        MACRO_EXPAND_STRING_VIEW(macro_value);
        return is_not_ ^ (std::get<std::string_view>(operand).ends_with(macro_value));
      }
    } else {
      return false;
    }
  }
};
} // namespace Operator
} // namespace SrSecurity