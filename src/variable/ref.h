/**
 * Copyright (c) 2024-2026 Stone Rhino and contributors.
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

#include "../macro/variable_macro.h"

namespace Wge {
namespace Variable {
class Ref final : public MatchedPTreeBase {
public:
  enum class RefType { MatchedOPTree, MatchedVPTree, CurrentOPTree, CurrentVPTree };

public:
  Ref(RefType ref_type, std::string&& key, std::string&& sub_name, bool is_not, bool is_counter,
      std::string_view curr_rule_file_path)
      : MatchedPTreeBase(std::move(sub_name), is_not, is_counter, curr_rule_file_path),
        ref_type_(ref_type), key_(std::move(key)) {}

  Ref(std::unique_ptr<Macro::VariableMacro>&& sub_name_macro, bool is_not, bool is_counter,
      std::string_view curr_rule_file_path)
      : MatchedPTreeBase("", is_not, is_counter, curr_rule_file_path) {
    // Does not support sub_name macro
    UNREACHABLE();
  }

public:
  const std::vector<const Common::PropertyTree*>&
  getAllMatchedPtrees(Transaction& t) const override {
    UNREACHABLE();
    static const std::vector<const Common::PropertyTree*> empty_result;
    return empty_result;
  }

  const Common::PropertyTree* getMatchedPTree(Transaction& t) const override {
    auto ref = t.getReference(key_);

    for (int i = 0; i < parent_count_; ++i) {
      ref = ref->parent();
      if (!ref) {
        WGE_LOG_WARN("Ref parent is nullptr!");
        break;
      }
    }

    return ref;
  }

public:
  FullName fullName() const override {
    switch (ref_type_) {
    case RefType::MatchedOPTree:
      return {matched_optree_main_name_, sub_name_};
    case RefType::MatchedVPTree:
      return {matched_vptree_main_name_, sub_name_};
    case RefType::CurrentOPTree:
      return {current_optree_main_name_, sub_name_};
    case RefType::CurrentVPTree:
      return {current_vptree_main_name_, sub_name_};
    default:
      UNREACHABLE();
    }
  }

  std::string_view mainName() const override {
    switch (ref_type_) {
    case RefType::MatchedOPTree:
      return matched_optree_main_name_;
    case RefType::MatchedVPTree:
      return matched_vptree_main_name_;
    case RefType::CurrentOPTree:
      return current_optree_main_name_;
    case RefType::CurrentVPTree:
      return current_vptree_main_name_;
    default:
      UNREACHABLE();
    }
  }

public:
  const std::string& key() const { return key_; }

private:
  RefType ref_type_;
  std::string key_;

  // The different main names for different ref types are necessary
  // They avoid conflicts when both MATCHED_OPTREE_REF and MATCHED_VPTREE_REF that have the same sub
  // name are used in the same rule
  static constexpr std::string_view matched_optree_main_name_{"MATCHED_OPTREE_REF"};
  static constexpr std::string_view matched_vptree_main_name_{"MATCHED_VPTREE_REF"};
  static constexpr std::string_view current_optree_main_name_{"CURRENT_OPTREE_REF"};
  static constexpr std::string_view current_vptree_main_name_{"CURRENT_VPTREE_REF"};
}; // namespace Variable
} // namespace Variable
} // namespace Wge