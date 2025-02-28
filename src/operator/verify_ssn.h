#pragma once

#include "operator_base.h"

namespace SrSecurity {
namespace Operator {
class VerifySSN : public OperatorBase {
  DECLARE_OPERATOR_NAME(verifySSN);

public:
  VerifySSN(std::string&& literal_value, bool is_not)
      : OperatorBase(std::move(literal_value), is_not) {}

public:
  bool evaluate(Transaction& t, const Common::Variant& operand) const override {
    assert(false);
    throw "Not implemented!";
  }
};
} // namespace Operator
} // namespace SrSecurity