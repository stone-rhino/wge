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
  Eq(std::string&& literal_value) : OperatorBase(std::move(literal_value)) {
    value_ = ::atoll(literal_value.c_str());
  }

public:
  bool evaluate(Transaction& t, std::string_view operand) const override {
    int64_t operand_value = 0;
    std::from_chars(operand.data(), operand.data() + operand.size(), operand_value);
    return value_ == operand_value;
  }

private:
  int64_t value_;
};
} // namespace Operator
} // namespace SrSecurity