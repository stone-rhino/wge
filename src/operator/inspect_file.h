#pragma once

#include "operator_base.h"

#include "../common/util.h"

namespace SrSecurity {
namespace Operator {
class InspectFile : public OperatorBase {
  DECLARE_OPERATOR_NAME(inspectFile);

public:
  InspectFile(std::string&& operator_value) : OperatorBase(std::move(operator_value)) {}

public:
  void preCompile() override { regex_expr_ = operator_value_; }
};
} // namespace Operator
} // namespace SrSecurity