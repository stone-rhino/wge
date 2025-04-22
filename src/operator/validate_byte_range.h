/**
 * Copyright (c) 2024-2025 Stone Rhino and contributors.
 *
 * MIT License (http://opensource.org/licenses/MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
 * associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
 * NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#pragma once

#include <bitset>

#include "operator_base.h"

namespace SrSecurity {
namespace Operator {
class ValidateByteRange : public OperatorBase {
  DECLARE_OPERATOR_NAME(validateByteRange);

public:
  ValidateByteRange(std::string&& literal_value, bool is_not, std::string_view curr_rule_file_path)
      : OperatorBase(std::move(literal_value), is_not) {
    // Split the literal value into tokens.
    std::vector<std::string_view> tokens = Common::SplitTokens(literal_value_);

    // Fill the byte range.
    for (auto& token : tokens) {
      auto pos = token.find('-');
      if (pos != std::string_view::npos) {
        std::string_view start, end;
        start = token.substr(0, pos);
        end = token.substr(pos + 1);
        uint32_t start_value, end_value;
        std::from_chars(start.data(), start.data() + start.size(), start_value);
        std::from_chars(end.data(), end.data() + end.size(), end_value);
        if (start_value < byte_range_.size() && end_value < byte_range_.size()) {
          for (uint32_t i = start_value; i <= end_value; ++i) {
            byte_range_.set(i);
          }
        }
      } else {
        uint32_t value;
        std::from_chars(token.data(), token.data() + token.size(), value);
        if (value < byte_range_.size()) {
          byte_range_.set(value);
        }
      }
    }
  }

  ValidateByteRange(const std::shared_ptr<Macro::MacroBase> macro, bool is_not,
                    std::string_view curr_rule_file_path)
      : OperatorBase(macro, is_not) {
    // Not supported
    UNREACHABLE();
  }

public:
  bool evaluate(Transaction& t, const Common::Variant& operand) const override {
    if (!IS_STRING_VIEW_VARIANT(operand)) [[unlikely]] {
      return false;
    }

    std::string_view operand_str = std::get<std::string_view>(operand);
    if (!is_not_) [[likely]] {
      for (auto& c : operand_str) {
        if (!byte_range_.test(static_cast<uint8_t>(c))) {
          return true;
        }
      }
      return false;
    } else {
      for (auto& c : operand_str) {
        if (byte_range_.test(static_cast<uint8_t>(c))) {
          return true;
        }
      }
      return false;
    }
  }

private:
  std::bitset<256> byte_range_;
};
} // namespace Operator
} // namespace SrSecurity