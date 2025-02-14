#pragma once

#include "operator_base.h"

namespace SrSecurity {
namespace Operator {
class DetectXSS : public OperatorBase {
  DECLARE_OPERATOR_NAME(detectXSS);

public:
  DetectXSS(std::string&& literal_value) : OperatorBase(std::move(literal_value)) {}

public:
  bool evaluate(Transaction& t, const std::string& value) const override { assert(false); }
};
} // namespace Operator
} // namespace SrSecurity