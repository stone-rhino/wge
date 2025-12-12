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
 * Returns true if the parameter string is found at the beginning of the input. Macro expansion is
 * performed on the parameter string before comparison.
 */
class BeginsWith final : public OperatorBase {
  DECLARE_OPERATOR_NAME(beginsWith);

public:
  BeginsWith(std::string&& literal_value, bool is_not, std::string_view curr_rule_file_path)
      : OperatorBase(std::move(literal_value), is_not) {}

  BeginsWith(std::unique_ptr<Macro::MacroBase>&& macro, bool is_not,
             std::string_view curr_rule_file_path)
      : OperatorBase(std::move(macro), is_not) {}

public:
  bool evaluate(Transaction& t, const Common::Variant& operand) const override {
    if (IS_STRING_VIEW_VARIANT(operand))
      [[likely]] {
        if (!macro_logic_matcher_)
          [[likely]] { return std::get<std::string_view>(operand).starts_with(literal_value_); }
        else {
          return macro_logic_matcher_->match(
              t, operand, empty_match_,
              [](Transaction& t, const Common::Variant& left_operand,
                 const Common::EvaluateElement& right_operand, void* user_data) {
                assert(IS_STRING_VIEW_VARIANT(right_operand.variant_));
                if (!IS_STRING_VIEW_VARIANT(right_operand.variant_)) {
                  return false;
                }

                bool matched = std::get<std::string_view>(left_operand)
                                   .starts_with(std::get<std::string_view>(right_operand.variant_));

                WGE_LOG_TRACE([&]() {
                  std::string sub_name;
                  if (!right_operand.variable_sub_name_.empty()) {
                    sub_name = std::format("\"{}\":", right_operand.variable_sub_name_);
                  }
                  return std::format("{} @{} {}{} => {}", std::get<std::string_view>(left_operand),
                                     name_, sub_name,
                                     std::get<std::string_view>(right_operand.variant_), matched);
                }());

                return matched;
              },
              nullptr);
        }
      }
    else {
      return false;
    }
  }
};
} // namespace Operator
} // namespace Wge