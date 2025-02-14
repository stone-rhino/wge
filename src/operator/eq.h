#pragma once

#include "operator_base.h"

#include "../common/util.h"

namespace SrSecurity {
namespace Operator {

/**
 * Performs numerical comparison and returns true if the input value is equal to the provided
 * parameter. Macro expansion is performed on the parameter string before comparison.
 */
class Eq : public OperatorBase {
  DECLARE_OPERATOR_NAME(eq);

public:
  Eq(std::string&& operator_value) : OperatorBase(std::move(operator_value)) {
    value_ = ::atoll(operator_value.c_str());
  }

public:
  bool evaluate(Transaction& t, Variable::VariableBase* variable) const override {
    const std::string& value = variable->evaluate(t);
    return value_ == ::atoll(value.c_str());
  }

private:
  int64_t value_;
};
} // namespace Operator
} // namespace SrSecurity