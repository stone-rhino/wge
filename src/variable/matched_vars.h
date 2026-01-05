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

namespace Wge {
namespace Variable {
class MatchedVarsBase : public CollectionBase {
public:
  MatchedVarsBase(std::string&& sub_name, bool is_not, bool is_counter,
                  std::string_view curr_rule_file_path)
      : CollectionBase(std::move(sub_name), is_not, is_counter, curr_rule_file_path) {}

protected:
  void evaluateCollectionCounter(Transaction& t, Common::EvaluateResults& result) const override {
    result.emplace_back(static_cast<int64_t>(getAllMatchedVariables(t).size()));
  }

  void evaluateSpecifyCounter(Transaction& t, Common::EvaluateResults& result) const override {
    evaluateCollectionCounter(t, result);
  }

protected:
  const std::vector<Wge::Transaction::MatchedVariable>&
  getAllMatchedVariables(Transaction& t) const {
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

    return t.getMatchedVariables(rule_chain_index);
  }
};

class MatchedVars final : public MatchedVarsBase {
  DECLARE_VIRABLE_NAME(MATCHED_VARS);

public:
  MatchedVars(std::string&& sub_name, bool is_not, bool is_counter,
              std::string_view curr_rule_file_path)
      : MatchedVarsBase(std::move(sub_name), is_not, is_counter, curr_rule_file_path) {}

protected:
  void evaluateCollection(Transaction& t, Common::EvaluateResults& result) const override {
    for (auto& matched_variable : getAllMatchedVariables(t)) {
      auto full_name = matched_variable.variable_->fullName();
      if (!hasExceptVariable(t, main_name_, full_name.sub_name_))
        [[likely]] { result.emplace_back(matched_variable.transformed_value_.variant_); }
    }
  }

  void evaluateSpecify(Transaction& t, Common::EvaluateResults& result) const override {
    evaluateCollection(t, result);
  }
};
} // namespace Variable
} // namespace Wge