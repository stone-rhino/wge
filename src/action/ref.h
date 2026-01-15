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

#include <variant>

#include "action_base.h"

#include "../variable/matched_optree.h"
#include "../variable/matched_vptree.h"

namespace Wge {
namespace Action {

class Ref final : public ActionBase {
  DECLARE_ACTION_NAME(ref);

public:
  Ref(ActionBase::Branch branch, std::string&& key,
      std::unique_ptr<Variable::MatchedPTreeBase>&& matched_ptree)
      : ActionBase(branch), key_(std::move(key)), matched_tree_(std::move(matched_ptree)) {}

public:
  void evaluate(Transaction& t) const override {
    const Common::PropertyTree* ptree = matched_tree_->getMatchedPTree(t);
    if (ptree) {
      t.setReference(key_, ptree);
      WGE_LOG_TRACE("ref: {} = {} -> {}", key_, static_cast<const void*>(ptree), ptree->dump());
    }
  }

private:
  const std::string key_;
  const std::unique_ptr<Variable::MatchedPTreeBase> matched_tree_;
};
} // namespace Action
} // namespace Wge