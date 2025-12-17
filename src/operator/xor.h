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

#include "operator_base.h"

namespace Wge {
namespace Operator {

/**
 * Performs numerical xor and returns true if the result is non-zero, otherwise false. Macro
 * expansion is performed on the parameter string before xor operation.
 */
class Xor final : public OperatorBase {
  DECLARE_OPERATOR_NAME(xor);

public:
  Xor(std::string&& literal_value, bool is_not, std::string_view curr_rule_file_path)
      : OperatorBase(std::move(literal_value), is_not) {
    right_value_ = ::atoll(literal_value_.c_str());
  }

  Xor(std::unique_ptr<Macro::MacroBase>&& macro, bool is_not, std::string_view curr_rule_file_path)
      : OperatorBase(std::move(macro), is_not) {}

public:
  bool evaluate(Transaction& t, const Common::Variant& operand) const override {
    if (!IS_INT_VARIANT(operand))
      [[unlikely]] { return false; }

    if (!macro_logic_matcher_)
      [[likely]] {
        int64_t left_value = std::get<int64_t>(operand);
        return left_value ^ right_value_;
      }
    else {
      return macro_logic_matcher_->match(
          t, operand, empty_match_,
          [](Transaction& t, const Common::Variant& left_operand,
             const Common::EvaluateElement& right_operand, void* user_data) {
            assert(IS_INT_VARIANT(right_operand.variant_));
            if (!IS_INT_VARIANT(right_operand.variant_)) {
              return false;
            }
            bool matched =
                std::get<int64_t>(left_operand) ^ std::get<int64_t>(right_operand.variant_);

            WGE_LOG_TRACE([&]() {
              std::string sub_name;
              if (!right_operand.variable_sub_name_.empty()) {
                sub_name = std::format("\"{}\":", right_operand.variable_sub_name_);
              }
              return std::format("{} @{} {}{} => {}", std::get<int64_t>(left_operand), name_,
                                 sub_name, std::get<int64_t>(right_operand.variant_), matched);
            }());

            return matched;
          },
          nullptr);
    }
  }

private:
  int64_t right_value_;
};
} // namespace Operator
} // namespace Wge