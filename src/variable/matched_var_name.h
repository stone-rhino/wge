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

#include "matched_var.h"

namespace Wge {
namespace Variable {
class MatchedVarName final : public MatchedVarBase {
  DECLARE_VIRABLE_NAME(MATCHED_VAR_NAME);

public:
  MatchedVarName(std::string&& sub_name, bool is_not, bool is_counter,
                 std::string_view curr_rule_file_path)
      : MatchedVarBase(std::move(sub_name), is_not, is_counter) {}

protected:
  void evaluateCollection(Transaction& t, Common::EvaluateResults& result) const override {
    auto matched_var = getMatchedVariable(t);
    if (matched_var) {
      if (matched_var->variable_->isCollection())
        [[likely]] {
          result.emplace_back(
              t.internString(std::format("{}:{}", matched_var->variable_->mainName(),
                                         matched_var->transformed_value_.variable_sub_name_)));
        }
      else {
        result.emplace_back(t.internString(matched_var->variable_->fullName().tostring()));
      }
    }
  }

  void evaluateSpecify(Transaction& t, Common::EvaluateResults& result) const override {
    evaluateCollection(t, result);
  }
};
} // namespace Variable
} // namespace Wge