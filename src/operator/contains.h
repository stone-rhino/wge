#pragma once

#include "operator_base.h"

namespace SrSecurity {
namespace Operator {
class Contains : public OperatorBase {
  DECLARE_OPERATOR_NAME(contains);

public:
  Contains(std::string&& literal_value, bool is_not)
      : OperatorBase(std::move(literal_value), is_not) {}

public:
  bool evaluate(Transaction& t, const Common::Variant& operand) const override {
    assert(false);
    throw "Not implemented!";
  }
};
} // namespace Operator
} // namespace SrSecurity