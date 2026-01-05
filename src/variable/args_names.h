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

#include "args.h"

namespace Wge {
namespace Variable {
class ArgsNames final : public ArgsBase {
  DECLARE_VIRABLE_NAME(ARGS_NAMES);

public:
  ArgsNames(std::string&& sub_name, bool is_not, bool is_counter,
            std::string_view curr_rule_file_path)
      : ArgsBase(std::move(sub_name), is_not, is_counter, curr_rule_file_path) {}

protected:
  void evaluateCollection(Transaction& t, Common::EvaluateResults& result) const override {
    auto& line_query_params = t.getRequestLineInfo().query_params_.getLinked();
    auto& body_query_params = getBodyQueryParams(t);

    for (auto& elem : line_query_params) {
      if (!hasExceptVariable(t, main_name_, elem.first))
        [[likely]] { result.emplace_back(elem.first, elem.first); }
    }
    for (auto& elem : body_query_params) {
      if (!hasExceptVariable(t, main_name_, elem.first))
        [[likely]] { result.emplace_back(elem.first, elem.first); }
    }
  }

  void evaluateSpecify(Transaction& t, Common::EvaluateResults& result) const override {
    if (!isRegex())
      [[likely]] {
        auto& line_query_params_map = t.getRequestLineInfo().query_params_.get();
        auto& body_query_params_map = getBodyQueryParamsMap(t);

        auto range = line_query_params_map.equal_range(sub_name_);
        for (auto iter = range.first; iter != range.second; ++iter) {
          result.emplace_back(iter->first);
        }
        auto range2 = body_query_params_map.equal_range(sub_name_);
        for (auto iter = range2.first; iter != range2.second; ++iter) {
          result.emplace_back(iter->first);
        }
      }
    else {
      auto& line_query_params = t.getRequestLineInfo().query_params_.getLinked();
      auto& body_query_params = getBodyQueryParams(t);

      for (auto& elem : line_query_params) {
        if (!hasExceptVariable(t, main_name_, elem.first))
          [[likely]] {
            if (match(elem.first)) {
              result.emplace_back(elem.first, elem.first);
            }
          }
      }
      for (auto& elem : body_query_params) {
        if (!hasExceptVariable(t, main_name_, elem.first))
          [[likely]] {
            if (match(elem.first)) {
              result.emplace_back(elem.first, elem.first);
            }
          }
      }
    }
  }
};
} // namespace Variable
} // namespace Wge