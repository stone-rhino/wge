#pragma once

#include "operator_base.h"

namespace SrSecurity {
namespace Operator {
class VerifySSN : public OperatorBase {
  DECLARE_OPERATOR_NAME(verifySSN);

public:
  VerifySSN(std::string&& literal_value) : OperatorBase(std::move(literal_value)) {}

public:
  bool evaluate(Transaction& t, std::string_view operand) const override { assert(false); throw "Not implemented!"; }
};
} // namespace Operator
} // namespace SrSecurity