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

#include "ptree.h"
#include "variable_base.h"

namespace Wge {
namespace Variable {
class MatchedPTreeBase : public VariableBase {
public:
  MatchedPTreeBase(std::string&& sub_name, bool is_not, bool is_counter,
                   std::string_view curr_rule_file_path)
      : VariableBase(std::move(sub_name), is_not, is_counter) {
    // Find count of '../' in sub_name
    parent_count_ = 0;
    size_t pos = 0;
    while ((pos = sub_name_.find("../", pos)) != std::string::npos) {
      ++parent_count_;
      pos += 3;
    }

    // Remove '../' from sub_name
    std::string no_parent_sub_name = sub_name_.substr(parent_count_ * 3);

    // Initialize paths
    PTree::initPaths(no_parent_sub_name, paths_);
  }

public:
  size_t parentCount() const { return parent_count_; }
  const std::vector<PTree::Path>& paths() const { return paths_; }

protected:
  size_t parent_count_;
  std::vector<PTree::Path> paths_;
};
} // namespace Variable
} // namespace Wge