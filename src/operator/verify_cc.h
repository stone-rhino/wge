#pragma once

#include "operator_base.h"

namespace SrSecurity {
namespace Operator {
class VerifyCC : public OperatorBase {
  DECLARE_OPERATOR_NAME(verifyCC);

public:
  VerifyCC(std::string&& literal_value, bool is_not, std::string_view curr_rule_file_path)
      : OperatorBase(std::move(literal_value), is_not) {}

  VerifyCC(const std::shared_ptr<Macro::MacroBase> macro, bool is_not,
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