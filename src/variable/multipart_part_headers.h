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
#include "evaluate_help.h"
#include "variable_base.h"

namespace SrSecurity {
namespace Variable {
class MultipartPartHeaders : public VariableBase, public CollectionBase {
  DECLARE_VIRABLE_NAME(MULTIPART_PART_HEADERS);

public:
  MultipartPartHeaders(std::string&& sub_name, bool is_not, bool is_counter)
      : VariableBase(std::move(sub_name), is_not, is_counter), CollectionBase(sub_name_) {
    if (sub_name_ == "_charset_") {
      is_charset_ = true;
    }
  }

public:
  void evaluate(Transaction& t, Common::EvaluateResults& result) const override {
    // _charset_ is a special case, it is used to get the charset of the multipart/form-data
    // content. It is not a header, we can get it from the name-value pair.
    if (is_charset_) {
      auto range = t.getBodyMultiPart().getNameValue().equal_range("_charset_");
      int count = 0;
      for (auto iter = range.first; iter != range.second; ++iter) {
        ++count;
      }

      if (is_counter_) {
        result.append(count);
      } else {
        for (auto iter = range.first; iter != range.second; ++iter) {
          result.append(iter->second);
        }
      }

      return;
    }

    RETURN_IF_COUNTER(
        // collection
        { result.append(static_cast<int>(t.getBodyMultiPart().getHeaders().size())); },
        // specify subname
        {
          auto iter_range = t.getBodyMultiPart().getHeaders().equal_range(sub_name_);
          int count = 0;
          for (auto iter = iter_range.first; iter != iter_range.second; ++iter) {
            ++count;
          }
          result.append(count);
        });

    RETURN_VALUE(
        // collection
        {
          for (auto& elem : t.getBodyMultiPart().getHeaders()) {
            if (!hasExceptVariable(elem.first)) [[likely]] {
              result.append(elem.second, elem.first);
            }
          }
        },
        // collection regex
        {
          for (auto& elem : t.getBodyMultiPart().getHeaders()) {
            if (!hasExceptVariable(elem.first)) [[likely]] {
              if (match(elem.first)) {
                result.append(elem.second, elem.first);
              }
            }
          }
        },
        // specify subname
        {
          if (!hasExceptVariable(sub_name_)) [[likely]] {
            auto iter_range = t.getBodyMultiPart().getHeaders().equal_range(sub_name_);
            for (auto iter = iter_range.first; iter != iter_range.second; ++iter) {
              result.append(iter->second);
            }
          }
        });
  }

private:
  bool is_charset_{false};
};
} // namespace Variable
} // namespace SrSecurity