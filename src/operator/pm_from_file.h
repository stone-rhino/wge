#pragma once

#include "operator_base.h"

namespace SrSecurity {
namespace Operator {
class PmFromFile : public OperatorBase {
  DECLARE_OPERATOR_NAME(pmFromFile);

public:
  PmFromFile(std::string&& literal_value) : OperatorBase(std::move(literal_value)) {}

public:
  bool evaluate(Transaction& t, const Common::Variant& operand) const override { assert(false); throw "Not implemented!"; }
};
} // namespace Operator
} // namespace SrSecurity