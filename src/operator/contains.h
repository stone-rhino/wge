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
class Contains final : public OperatorBase {
  DECLARE_OPERATOR_NAME(contains);

public:
  Contains(std::string&& literal_value, bool is_not, std::string_view curr_rule_file_path)
      : OperatorBase(std::move(literal_value), is_not) {}

  Contains(std::unique_ptr<Macro::MacroBase>&& macro, bool is_not,
           std::string_view curr_rule_file_path)
      : OperatorBase(std::move(macro), is_not) {}

public:
  bool evaluate(Transaction& t, const Common::Variant& operand) const override {
    bool matched = false;
    if (IS_STRING_VIEW_VARIANT(operand))
      [[likely]] {
        if (!macro_logic_matcher_)
          [[likely]] {
            matched =
                std::get<std::string_view>(operand).find(literal_value_) != std::string_view::npos;
            if (matched) {
              t.stageCapture(0, literal_value_);
            }
          }
        else {
          std::vector<std::string_view> captrue_array;
          matched = macro_logic_matcher_->match(
              t, operand, empty_match_,
              [](Transaction& t, const Common::Variant& left_operand,
                 const Common::EvaluateElement& right_operand, void* user_data) {
                assert(IS_STRING_VIEW_VARIANT(right_operand.variant_));
                if (!IS_STRING_VIEW_VARIANT(right_operand.variant_)) {
                  return false;
                }

                std::vector<std::string_view>& captrue_array =
                    *static_cast<std::vector<std::string_view>*>(user_data);

                bool matched = std::get<std::string_view>(left_operand)
                                   .find(std::get<std::string_view>(right_operand.variant_)) !=
                               std::string_view::npos;
                if (matched) {
                  captrue_array.emplace_back(std::get<std::string_view>(right_operand.variant_));
                }

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
              &captrue_array);

          if (matched) {
            for (size_t i = 0; i < captrue_array.size(); ++i) {
              t.stageCapture(i, captrue_array[i]);
            }
          }
        }
      }

    return matched;
  }
};
} // namespace Operator
} // namespace Wge