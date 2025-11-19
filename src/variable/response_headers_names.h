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
class ResponseHeadersNames final : public VariableBase, public CollectionBase {
  DECLARE_VIRABLE_NAME(RESPONSE_HEADERS_NAMES);

public:
  ResponseHeadersNames(std::string&& sub_name, bool is_not, bool is_counter,
                       std::string_view curr_rule_file_path)
      : VariableBase(std::move(sub_name), is_not, is_counter),
        CollectionBase(sub_name_, curr_rule_file_path) {}

public:
  void evaluate(Transaction& t, Common::EvaluateResults& result) const override {
    RETURN_IF_COUNTER(
        // collection
        { result.emplace_back(static_cast<int64_t>(t.httpExtractor().response_header_count_)); },
        // specify subname
        {
          result.emplace_back(
              static_cast<int64_t>(t.httpExtractor().response_header_find_(sub_name_).size()));
        });

    RETURN_VALUE(
        // collection
        {
          t.httpExtractor().response_header_traversal_(
              [&](std::string_view key, std::string_view value) {
                if (!hasExceptVariable(t, main_name_, key))
                  [[likely]] { result.emplace_back(key, key); }
                return true;
              });
        },
        // collection regex
        {
          t.httpExtractor().response_header_traversal_(
              [&](std::string_view key, std::string_view value) {
                if (!hasExceptVariable(t, main_name_, key))
                  [[likely]] {
                    if (match(key)) {
                      result.emplace_back(key, key);
                    }
                  }
                return true;
              });
        },
        // specify subname
        {
          std::vector<std::string_view> values = t.httpExtractor().response_header_find_(sub_name_);
          for (size_t i = 0; i < values.size(); ++i) {
            result.emplace_back(sub_name_);
          }
        });
  }

  bool isCollection() const override { return sub_name_.empty(); };
};
} // namespace Variable
} // namespace Wge