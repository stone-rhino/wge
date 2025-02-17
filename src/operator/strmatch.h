#pragma once

#include "operator_base.h"

namespace SrSecurity {
namespace Operator {
class Strmatch : public OperatorBase {
  DECLARE_OPERATOR_NAME(strmatch);

public:
  Strmatch(std::string&& literal_value) : OperatorBase(std::move(literal_value)) {}

public:
  bool evaluate(Transaction& t, std::string_view operand) const override { assert(false); }
};
} // namespace Operator
} // namespace SrSecurity