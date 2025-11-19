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

#include "collection_base.h"
#include "variable_base.h"

namespace Wge {
namespace Variable {
class MatchedVarsNames final : public VariableBase, public CollectionBase {
  DECLARE_VIRABLE_NAME(MATCHED_VARS_NAMES);

public:
  MatchedVarsNames(std::string&& sub_name, bool is_not, bool is_counter,
                   std::string_view curr_rule_file_path)
      : VariableBase(std::move(sub_name), is_not, is_counter),
        CollectionBase(sub_name_, curr_rule_file_path) {}

public:
  void evaluate(Transaction& t, Common::EvaluateResults& result) const override {
    // If the current evaluate rule is a chained rule, we should get the matched variable from the
    // parent rule. If the current evaluate rule is not a chained rule, we should get the matched
    // variable from the current rule.
    int rule_chain_index = -1;
    if (t.getCurrentEvaluateRule()) {
      rule_chain_index = t.getCurrentEvaluateRule()->chainIndex();
      if (rule_chain_index >= 0) {
        rule_chain_index--;
      }
    }

    RETURN_IF_COUNTER(
        // collection
        {
          result.emplace_back(static_cast<int64_t>(t.getMatchedVariables(rule_chain_index).size()));
        },
        // specify subname
        { UNREACHABLE(); });

    RETURN_VALUE(
        // collection
        {
          for (auto& matched_variable : t.getMatchedVariables(rule_chain_index)) {
            auto full_name = matched_variable.variable_->fullName();
            if (!hasExceptVariable(t, main_name_, full_name.sub_name_))
              [[likely]] {
                if (matched_variable.variable_->isCollection()) {
                  result.emplace_back(t.internString(
                      std::format("{}:{}", matched_variable.variable_->mainName(),
                                  matched_variable.transformed_value_.variable_sub_name_)));
                } else {
                  result.emplace_back(t.internString(full_name.tostring()));
                }
              }
          }
        },
        // collection regex
        { UNREACHABLE(); },
        // specify subname
        { UNREACHABLE(); });
  }

  bool isCollection() const override { return sub_name_.empty(); };
};
} // namespace Variable
} // namespace Wge