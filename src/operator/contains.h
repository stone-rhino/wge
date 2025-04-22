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