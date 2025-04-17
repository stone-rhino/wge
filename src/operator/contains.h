#pragma once

#include "operator_base.h"

namespace SrSecurity {
namespace Operator {
class Contains : public OperatorBase {
  DECLARE_OPERATOR_NAME(contains);

public:
  Contains(std::string&& literal_value, bool is_not, std::string_view curr_rule_file_path)
      : OperatorBase(std::move(literal_value), is_not) {}

  Contains(const std::shared_ptr<Macro::MacroBase> macro, bool is_not,
           std::string_view curr_rule_file_path)
      : OperatorBase(macro, is_not) {}

public:
  bool evaluate(Transaction& t, const Common::Variant& operand) const override {
    bool matched = false;
    if (IS_STRING_VIEW_VARIANT(operand)) [[likely]] {
      if (!macro_) [[likely]] {
        matched = is_not_ ^ (std::get<std::string_view>(operand).find(literal_value_) !=
                             std::string_view::npos);
        Common::EvaluateResults::Element value;
        value.variant_ = literal_value_;
        t.addCapture(std::move(value));
      } else {
        MACRO_EXPAND_STRING_VIEW(macro_value);
        matched = is_not_ ^
                  (std::get<std::string_view>(operand).find(macro_value) != std::string_view::npos);
        Common::EvaluateResults::Element value;
        value.string_buffer_ = macro_value;
        value.variant_ = value.string_buffer_;
        t.addCapture(std::move(value));
      }
    }

    return matched;
  }
};
} // namespace Operator
} // namespace SrSecurity