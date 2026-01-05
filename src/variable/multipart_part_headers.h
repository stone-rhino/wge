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
class MultipartPartHeaders final : public CollectionBase {
  DECLARE_VIRABLE_NAME(MULTIPART_PART_HEADERS);

public:
  MultipartPartHeaders(std::string&& sub_name, bool is_not, bool is_counter,
                       std::string_view curr_rule_file_path)
      : CollectionBase(std::move(sub_name), is_not, is_counter, curr_rule_file_path) {
    if (sub_name_ == "_charset_") {
      is_charset_ = true;
    }
  }

protected:
  void evaluateCollectionCounter(Transaction& t, Common::EvaluateResults& result) const override {
    result.emplace_back(static_cast<int64_t>(t.getBodyMultiPart().getHeaders().size()));
  }

  void evaluateSpecifyCounter(Transaction& t, Common::EvaluateResults& result) const override {
    int64_t count = t.getBodyMultiPart().getHeaders().count(sub_name_);
    result.emplace_back(count);
  }

  void evaluateCollection(Transaction& t, Common::EvaluateResults& result) const override {
    for (auto& elem : t.getBodyMultiPart().getHeaders()) {
      if (!hasExceptVariable(t, main_name_, elem.first))
        [[likely]] { result.emplace_back(elem.second, elem.first); }
    }
  }

  void evaluateSpecify(Transaction& t, Common::EvaluateResults& result) const override {
    if (!isRegex())
      [[likely]] {
        if (is_charset_) {
          // _charset_ is a special case, it is used to get the charset of the multipart/form-data
          // content. It is not a header, we can get it from the name-value pair.
          static constexpr std::string_view key = "_charset_";
          auto range = t.getBodyMultiPart().getNameValue().equal_range(key);
          for (auto iter = range.first; iter != range.second; ++iter) {
            result.emplace_back(iter->second);
          }
        } else {
          auto iter_range = t.getBodyMultiPart().getHeaders().equal_range(sub_name_);
          for (auto iter = iter_range.first; iter != iter_range.second; ++iter) {
            result.emplace_back(iter->second);
          }
        }
      }
    else {
      for (auto& elem : t.getBodyMultiPart().getHeaders()) {
        if (!hasExceptVariable(t, main_name_, elem.first))
          [[likely]] {
            if (match(elem.first)) {
              result.emplace_back(elem.second, elem.first);
            }
          }
      }
    }
  }

private:
  bool is_charset_{false};
};
} // namespace Variable
} // namespace Wge