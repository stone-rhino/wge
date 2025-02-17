#pragma once

#include "operator_base.h"

namespace SrSecurity {
namespace Operator {
class Lt : public OperatorBase {
  DECLARE_OPERATOR_NAME(lt);

public:
  Lt(std::string&& literal_value) : OperatorBase(std::move(literal_value)) {
    value_ = ::atoll(literal_value.c_str());
  }

public:
  bool evaluate(Transaction& t, std::string_view value) const override {
    int64_t operand_value = 0;
    std::from_chars(value.data(), value.data() + value.size(), operand_value);
    return operand_value < value_;
  }

private:
  int64_t value_;
};
} // namespace Operator
} // namespace SrSecurity