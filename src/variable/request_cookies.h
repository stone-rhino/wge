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
#include "request_headers.h"

namespace Wge {
namespace Variable {
class RequestCookiesBase : public CollectionBase {
public:
  RequestCookiesBase(std::string&& sub_name, bool is_not, bool is_counter,
                     std::string_view curr_rule_file_path)
      : CollectionBase(std::move(sub_name), is_not, is_counter, curr_rule_file_path) {}

protected:
  void evaluateCollectionCounter(Transaction& t, Common::EvaluateResults& result) const override {
    const std::unordered_multimap<std::string_view, std::string_view>& cookies = t.getCookies();

    result.emplace_back(static_cast<int64_t>(cookies.size()));
  }

  void evaluateSpecifyCounter(Transaction& t, Common::EvaluateResults& result) const override {
    const std::unordered_multimap<std::string_view, std::string_view>& cookies = t.getCookies();

    int64_t count = cookies.count(sub_name_);
    result.emplace_back(count);
  }
};

class RequestCookies final : public RequestCookiesBase {
  DECLARE_VIRABLE_NAME(REQUEST_COOKIES);

public:
  RequestCookies(std::string&& sub_name, bool is_not, bool is_counter,
                 std::string_view curr_rule_file_path)
      : RequestCookiesBase(std::move(sub_name), is_not, is_counter, curr_rule_file_path) {}

protected:
  void evaluateCollection(Transaction& t, Common::EvaluateResults& result) const override {
    const std::unordered_multimap<std::string_view, std::string_view>& cookies = t.getCookies();

    for (auto& elem : cookies) {
      if (!hasExceptVariable(t, main_name_, elem.first))
        [[likely]] { result.emplace_back(elem.second, elem.first); }
    }
  }

  void evaluateSpecify(Transaction& t, Common::EvaluateResults& result) const override {
    const std::unordered_multimap<std::string_view, std::string_view>& cookies = t.getCookies();

    if (!isRegex())
      [[likely]] {
        auto range = cookies.equal_range(sub_name_);
        for (auto it = range.first; it != range.second; ++it) {
          result.emplace_back(it->second);
        }
      }
    else {
      for (auto& elem : cookies) {
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