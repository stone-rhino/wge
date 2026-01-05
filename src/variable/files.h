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
class FilesBase : public CollectionBase {
public:
  FilesBase(std::string&& sub_name, bool is_not, bool is_counter,
            std::string_view curr_rule_file_path)
      : CollectionBase(std::move(sub_name), is_not, is_counter, curr_rule_file_path) {}

protected:
  void evaluateCollectionCounter(Transaction& t, Common::EvaluateResults& result) const override {
    auto& filename = t.getBodyMultiPart().getNameFileNameLinked();

    result.emplace_back(static_cast<int64_t>(filename.size()));
  }

  void evaluateSpecifyCounter(Transaction& t, Common::EvaluateResults& result) const override {
    auto& filename_map = t.getBodyMultiPart().getNameFileName();

    int64_t count = filename_map.count(sub_name_);
    result.emplace_back(count);
  }
};

class Files final : public FilesBase {
  DECLARE_VIRABLE_NAME(FILES);

public:
  Files(std::string&& sub_name, bool is_not, bool is_counter, std::string_view curr_rule_file_path)
      : FilesBase(std::move(sub_name), is_not, is_counter, curr_rule_file_path) {}

protected:
  void evaluateCollection(Transaction& t, Common::EvaluateResults& result) const override {
    auto& filename = t.getBodyMultiPart().getNameFileNameLinked();

    for (auto& elem : filename) {
      if (!hasExceptVariable(t, main_name_, elem.first))
        [[likely]] { result.emplace_back(elem.second, elem.first); }
    }
  }

  void evaluateSpecify(Transaction& t, Common::EvaluateResults& result) const override {
    if (!isRegex())
      [[likely]] {
        auto& filename_map = t.getBodyMultiPart().getNameFileName();

        auto iter = filename_map.find(sub_name_);
        if (iter != filename_map.end()) {
          result.emplace_back(iter->second);
        }
      }
    else {
      auto& filename = t.getBodyMultiPart().getNameFileNameLinked();

      for (auto& elem : filename) {
        if (!hasExceptVariable(t, main_name_, elem.first))
          [[likely]] {
            if (match(elem.first)) {
              result.emplace_back(elem.second, elem.first);
            }
          }
      }
    }
  }
};
} // namespace Variable
} // namespace Wge