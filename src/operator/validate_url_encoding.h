#pragma once

#include "operator_base.h"

namespace SrSecurity {
namespace Operator {
/**
 * Validates the URL-encoded characters in the provided input string.
 */
class ValidateUrlEncoding : public OperatorBase {
  DECLARE_OPERATOR_NAME(validateUrlEncoding);

public:
  ValidateUrlEncoding(std::string&& literal_value, bool is_not,
                      std::string_view curr_rule_file_path)
      : OperatorBase(std::move(literal_value), is_not) {}

  ValidateUrlEncoding(const std::shared_ptr<Macro::MacroBase> macro, bool is_not,
                      std::string_view curr_rule_file_path)
      : OperatorBase(macro, is_not) {}

public:
  bool evaluate(Transaction& t, const Common::Variant& operand) const override {
    if (!IS_STRING_VIEW_VARIANT(operand)) [[unlikely]] {
      return false;
    }

    // Check that after percent character must be two hexadecimal characters.
    std::string_view input = std::get<std::string_view>(operand);
    for (size_t i = 0; i < input.size(); ++i) {
      if (input[i] == '%' && i + 2 < input.size()) {
        if (!::isxdigit(input[i + 1]) || !::isxdigit(input[i + 2])) {
          return false;
        }
      }
    }

    return true;
  }
};
} // namespace Operator
} // namespace SrSecurity