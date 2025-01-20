#pragma once

#include "operator_base.h"

namespace SrSecurity {
namespace Operator {
class BeginsWith : public OperatorBase {
  DECLARE_OPERATOR_NAME(beginsWith);

public:
  BeginsWith(std::string&& operator_value) : OperatorBase(std::move(operator_value)) {}

public:
  void preCompile() override { regex_expr_ = operator_value_; }
};
} // namespace Operator
} // namespace SrSecurity