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
 * Performs numerical comparison and returns true if the input value is equal to the provided
 * parameter. Macro expansion is performed on the parameter string before comparison.
 */
class Eq final : public OperatorBase {
  DECLARE_OPERATOR_NAME(eq);

public:
  Eq(std::string&& literal_value, bool is_not, std::string_view curr_rule_file_path)
      : OperatorBase(std::move(literal_value), is_not) {
    right_value_ = ::atoll(literal_value_.c_str());
  }

  Eq(std::unique_ptr<Macro::MacroBase>&& macro, bool is_not, std::string_view curr_rule_file_path)
      : OperatorBase(std::move(macro), is_not) {}

public:
  void evaluate(Transaction& t, const Common::Variant& operand, Results& results) const override {
    if (!macro_)
      [[likely]] {
        if (IS_INT_VARIANT(operand))
          [[likely]] {
            int64_t left_value = std::get<int64_t>(operand);
            results.emplace_back(left_value == right_value_);
          }
        else {
          results.emplace_back(false);
        }
      }
    else {
      Common::EvaluateResults macro_result;
      macro_->evaluate(t, macro_result);
      if (macro_result.empty()) {
        results.emplace_back(empty_match_);
        return;
      }

      for (const auto& right_operand : macro_result) {
        if (IS_INT_VARIANT(right_operand.variant_))
          [[likely]] {
            if (!IS_INT_VARIANT(operand))
              [[unlikely]] {
                results.emplace_back(false);
                continue;
              }

            results.emplace_back(std::get<int64_t>(operand) ==
                                 std::get<int64_t>(right_operand.variant_));
            WGE_LOG_TRACE([&]() {
              std::string sub_name;
              if (!right_operand.variable_sub_name_.empty()) {
                sub_name = std::format("\"{}\":", right_operand.variable_sub_name_);
              }
              return std::format("{} @{} {}{} => {}", std::get<int64_t>(operand), name_, sub_name,
                                 std::get<int64_t>(right_operand.variant_),
                                 results.back().matched_);
            }());
          }
        else if (IS_EMPTY_VARIANT(right_operand.variant_)) {
          results.emplace_back(empty_match_);
        } else {
          results.emplace_back(false);
        }
      }
    }
  }

private:
  int64_t right_value_;
};
} // namespace Operator
} // namespace Wge