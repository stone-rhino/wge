#pragma once

#include "operator_base.h"

namespace SrSecurity {
namespace Operator {
class ContainsWord : public OperatorBase {
  DECLARE_OPERATOR_NAME(containsWord);

public:
  ContainsWord(std::string&& literal_value, bool is_not, std::string_view curr_rule_file_path)
      : OperatorBase(std::move(literal_value), is_not) {}

  ContainsWord(const std::shared_ptr<Macro::MacroBase> macro, bool is_not,
               std::string_view curr_rule_file_path)
      : OperatorBase(macro, is_not) {}

public:
  bool evaluate(Transaction& t, const Common::Variant& operand) const override {
    assert(false);
    throw "Not implemented!";
  }
};
} // namespace Operator
} // namespace SrSecurity