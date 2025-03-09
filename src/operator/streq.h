#pragma once

#include "operator_base.h"

namespace SrSecurity {
namespace Operator {
class Streq : public OperatorBase {
  DECLARE_OPERATOR_NAME(streq);

public:
  Streq(std::string&& literal_value, bool is_not)
      : OperatorBase(std::move(literal_value), is_not) {}

  Streq(const std::shared_ptr<Macro::MacroBase> macro, bool is_not) : OperatorBase(macro, is_not) {}

public:
  bool evaluate(Transaction& t, const Common::Variant& operand) const override {
    if (IS_STRING_VIEW_VARIANT(operand)) [[likely]] {
      return is_not_ ^ (literal_value_ == std::get<std::string_view>(operand));
    } else {
      return false;
    }
  }
};
} // namespace Operator
} // namespace SrSecurity