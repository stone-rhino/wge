#pragma once

#include "operator_base.h"
/**
 * Returns true if the input value (the needle) is found anywhere within the @within parameter (the
 * haystack). Macro expansion is performed on the parameter string before comparison.
 */
namespace SrSecurity {
namespace Operator {
class Within : public OperatorBase {
  DECLARE_OPERATOR_NAME(within);

public:
  Within(std::string&& literal_value, bool is_not)
      : OperatorBase(std::move(literal_value), is_not) {
    // Split the literal value into tokens.
    tokens_ = SplitTokens(literal_value_);
  }

  Within(const std::shared_ptr<Macro::MacroBase> macro, bool is_not)
      : OperatorBase(macro, is_not) {}

public:
  bool evaluate(Transaction& t, const Common::Variant& operand) const override {
    if (!IS_STRING_VIEW_VARIANT(operand)) [[unlikely]] {
      return false;
    }

    bool find = false;
    if (!macro_) [[likely]] {
      for (auto token : tokens_) {
        if (std::get<std::string_view>(operand).find(token) != std::string_view::npos) {
          find = true;
          break;
        }
      }
      return is_not_ ^ find;
    } else {
      MACRO_EXPAND_STRING_VIEW(macro_value);
      std::vector<std::string_view> tokens = SplitTokens(macro_value);
      for (auto token : tokens) {
        if (std::get<std::string_view>(operand).find(token) != std::string_view::npos) {
          find = true;
          break;
        }
      }
      return is_not_ ^ find;
    }
  }

private:
  std::vector<std::string_view> SplitTokens(std::string_view value) const {
    // Split by space
    constexpr char delimiter = ' ';

    std::vector<std::string_view> tokens;
    size_t pos = 0;
    size_t next_pos = 0;

    while ((next_pos = value.find(delimiter, pos)) != std::string_view::npos) {
      tokens.emplace_back(value.substr(pos, next_pos - pos));
      pos = next_pos + 1;
    }

    if (pos < value.size()) {
      tokens.emplace_back(value.substr(pos));
    }

    return tokens;
  }

private:
  std::vector<std::string_view> tokens_;
};
} // namespace Operator
} // namespace SrSecurity