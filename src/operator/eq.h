#pragma once

#include "operator_base.h"

namespace SrSecurity {
namespace Operator {

/**
 * Performs numerical comparison and returns true if the input value is equal to the provided
 * parameter. Macro expansion is performed on the parameter string before comparison.
 */
class Eq : public OperatorBase {
  DECLARE_OPERATOR_NAME(eq);

public:
  Eq(std::string&& literal_value, bool is_not) : OperatorBase(std::move(literal_value), is_not) {
    if (!macro_) {
      value_ = ::atoll(literal_value_.c_str());
    }
  }

public:
  bool evaluate(Transaction& t, const Common::Variant& operand) const override {
    if (IS_EMPTY_VARIANT(operand)) [[unlikely]] {
      return false;
    }

    int64_t operand_value = std::get<int>(operand);
    if (macro_) {
      int64_t macro_value = std::get<int>(macro_->evaluate(t));
      return is_not_ ? !(macro_value == operand_value) : macro_value == operand_value;
    } else {
      return is_not_ ? !(value_ == operand_value) : value_ == operand_value;
    }
  }

private:
  int64_t value_;
};
} // namespace Operator
} // namespace SrSecurity