#pragma once

#include "operator_base.h"

namespace SrSecurity {
namespace Operator {
class Ge : public OperatorBase {
  DECLARE_OPERATOR_NAME(ge);

public:
  Ge(std::string&& literal_value) : OperatorBase(std::move(literal_value)) {
    value_ = ::atoll(literal_value.c_str());
  }

public:
  bool evaluate(Transaction& t, std::string_view operand) const override {
    int64_t operand_value = 0;
    std::from_chars(operand.data(), operand.data() + operand.size(), operand_value);
    return operand_value >= value_;
  }

private:
  int64_t value_;
};
} // namespace Operator
} // namespace SrSecurity