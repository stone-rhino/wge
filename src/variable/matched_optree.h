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

#include "matched_ptree_base.h"

namespace Wge {
namespace Variable {
class MatchedOPTree final : public MatchedPTreeBase {
  DECLARE_VIRABLE_NAME(MATCHED_OPTREE);

public:
  MatchedOPTree(std::string&& sub_name, bool is_not, bool is_counter,
                std::string_view curr_rule_file_path)
      : MatchedPTreeBase(std::move(sub_name), is_not, is_counter, curr_rule_file_path) {}

protected:
  void evaluateCollectionCounter(Transaction& t, Common::EvaluateResults& result) const override {
    result.emplace_back(getAllMatchedOPTrees(t).empty() ? 0 : 1);
  }

  void evaluateSpecifyCounter(Transaction& t, Common::EvaluateResults& result) const override {
    evaluateCollectionCounter(t, result);
  }

  void evaluateCollection(Transaction& t, Common::EvaluateResults& result) const override {
    const Common::PropertyTree* matched_optree = getMatchedOPTree(t);
    if (matched_optree) {
      if (paths_.empty()) {
        Variable::PTree::evaluateNode(matched_optree, result);
      } else {
        Variable::PTree::evaluateNode(matched_optree, paths_, 0, result);
      }
    }
  }

  void evaluateSpecify(Transaction& t, Common::EvaluateResults& result) const override {
    evaluateCollection(t, result);
  }

private:
  const std::vector<const Common::PropertyTree*>& getAllMatchedOPTrees(Transaction& t) const {
    // If the current evaluate rule is a chained rule, we should get the matched vptree from the
    // parent rule. If the current evaluate rule is not a chained rule, we should get the matched
    // variable from the current rule.
    int rule_chain_index = -1;
    if (t.getCurrentEvaluateRule()) {
      rule_chain_index = t.getCurrentEvaluateRule()->chainIndex();
      if (rule_chain_index >= 0) {
        rule_chain_index--;
      }
    }

    return t.getMatchedOPTrees(rule_chain_index);
  }

  const Common::PropertyTree* getMatchedOPTree(Transaction& t) const {
    auto& all_matched_optrees = getAllMatchedOPTrees(t);
    if (all_matched_optrees.empty()) {
      return nullptr;
    }

    auto matched_optree = all_matched_optrees.back();
    WGE_LOG_TRACE("MatchedOPTree: {}", matched_optree->dump());

    for (int i = 0; i < parent_count_; ++i) {
      auto parent = matched_optree->parent();
      if (parent) {
        matched_optree = parent;
        WGE_LOG_TRACE("MatchedOPTree parent: {}", matched_optree->dump());
      } else {
        break;
      }
    }

    return matched_optree;
  }
};
} // namespace Variable
} // namespace Wge