#pragma once

#include "operator_base.h"

namespace SrSecurity {
namespace Operator {
class Le : public OperatorBase {
  DECLARE_OPERATOR_NAME(le);

public:
  Le(std::string&& literal_value, bool is_not, std::string_view curr_rule_file_path)
      : OperatorBase(std::move(literal_value), is_not) {
    value_ = ::atoll(literal_value_.c_str());
  }

  Le(const std::shared_ptr<Macro::MacroBase> macro, bool is_not,
     std::string_view curr_rule_file_path)
      : OperatorBase(macro, is_not) {}

public:
  bool evaluate(Transaction& t, const Common::Variant& operand) const override {
    if (!IS_INT_VARIANT(operand)) [[unlikely]] {
      return false;
    }

    int64_t operand_value = std::get<int>(operand);
    if (!macro_) [[likely]] {
      return is_not_ ^ (value_ <= operand_value);
    } else {
      MACRO_EXPAND_INT(macro_value);
      return is_not_ ^ (macro_value <= operand_value);
    }
  }

private:
  int64_t value_;
};
} // namespace Operator
} // namespace SrSecurity