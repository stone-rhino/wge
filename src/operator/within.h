#pragma once

#include "operator_base.h"

namespace SrSecurity {
namespace Operator {
class Within : public OperatorBase {
  DECLARE_OPERATOR_NAME(within);

public:
  Within(std::string&& literal_value, bool is_not)
      : OperatorBase(std::move(literal_value), is_not) {}

  Within(const std::shared_ptr<Macro::MacroBase> macro, bool is_not)
      : OperatorBase(macro, is_not) {}

public:
  bool evaluate(Transaction& t, const Common::Variant& operand) const override {
    assert(false);
    throw "Not implemented!";
  }
};
} // namespace Operator
} // namespace SrSecurity