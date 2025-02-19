#pragma once

#include "operator_base.h"

namespace SrSecurity {
namespace Operator {
class Le : public OperatorBase {
  DECLARE_OPERATOR_NAME(le);

public:
  Le(std::string&& literal_value) : OperatorBase(std::move(literal_value)) {
    if (!macro_) {
      value_ = ::atoll(literal_value_.c_str());
    }
  }

public:
  bool evaluate(Transaction& t, std::string_view operand) const override {
    int64_t operand_value = 0;
    std::from_chars(operand.data(), operand.data() + operand.size(), operand_value);
    if (macro_) {
      int64_t macro_value = 0;
      std::string_view macro_string = macro_->evaluate(t);
      std::from_chars(macro_string.data(), macro_string.data() + macro_string.size(), macro_value);
      return macro_value <= operand_value;
    } else {
      return value_ <= operand_value;
    }
  }

private:
  int64_t value_;
};
} // namespace Operator
} // namespace SrSecurity