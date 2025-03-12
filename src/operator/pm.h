#pragma once

#include "operator_base.h"
#include "within.h"

namespace SrSecurity {
namespace Operator {
class Pm : public OperatorBase {
  DECLARE_OPERATOR_NAME(pm);

public:
  Pm(std::string&& literal_value, bool is_not, std::string_view curr_rule_file_path)
      : OperatorBase(std::move(literal_value), is_not),
        within_(std::string(literal_value_), is_not, curr_rule_file_path) {}

  Pm(const std::shared_ptr<Macro::MacroBase> macro, bool is_not,
     std::string_view curr_rule_file_path)
      : OperatorBase(macro, is_not),
        within_(std::string(literal_value_), is_not, curr_rule_file_path) {
    // Not supported
    UNREACHABLE();
  }

public:
  bool evaluate(Transaction& t, const Common::Variant& operand) const override {
    return within_.evaluate(t, operand);
  }

private:
  Within within_;
};
} // namespace Operator
} // namespace SrSecurity