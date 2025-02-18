#pragma once

#include "operator_base.h"

namespace SrSecurity {
namespace Operator {
class Contains : public OperatorBase {
  DECLARE_OPERATOR_NAME(contains);

public:
  Contains(std::string&& literal_value) : OperatorBase(std::move(literal_value)) {}

public:
  bool evaluate(Transaction& t, std::string_view operand) const override { assert(false); throw "Not implemented!"; }
};
} // namespace Operator
} // namespace SrSecurity