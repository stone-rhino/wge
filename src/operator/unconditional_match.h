#pragma once

#include "operator_base.h"

namespace SrSecurity {
namespace Operator {
class UnconditionalMatch : public OperatorBase {
  DECLARE_OPERATOR_NAME(unconditionalMatch);

public:
  UnconditionalMatch(std::string&& literal_value, bool is_not, std::string_view curr_rule_file_path)
      : OperatorBase(std::move(literal_value), is_not) {}

  UnconditionalMatch(const std::shared_ptr<Macro::MacroBase> macro, bool is_not,
                     std::string_view curr_rule_file_path)
      : OperatorBase(macro, is_not) {}

public:
  bool evaluate(Transaction& t, const Common::Variant& operand) const override { return true; }
};
} // namespace Operator
} // namespace SrSecurity