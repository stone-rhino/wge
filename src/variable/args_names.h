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

#include "args_get_names.h"
#include "args_post_names.h"
#include "collection_base.h"
#include "variable_base.h"

namespace Wge {
namespace Variable {
class ArgsNames : public VariableBase, public CollectionBase {
  DECLARE_VIRABLE_NAME(ARGS_NAMES);

public:
  ArgsNames(std::string&& sub_name, bool is_not, bool is_counter)
      : VariableBase(std::move(sub_name), is_not, is_counter), CollectionBase(sub_name_) {}

public:
  void evaluate(Transaction& t, Common::EvaluateResults& result) const override {
    auto& line_query_params = t.getRequestLineInfo().query_params_.getLinked();
    auto& line_query_params_map = t.getRequestLineInfo().query_params_.get();

    // Get the query params by the request body processor type
    const std::vector<std::pair<std::string_view, std::string_view>>* body_query_params = nullptr;
    const std::unordered_multimap<std::string_view, std::string_view>* body_query_params_map =
        nullptr;
    switch (t.getRequestBodyProcessor()) {
    case BodyProcessorType::UrlEncoded:
      body_query_params = &t.getBodyQueryParam().getLinked();
      body_query_params_map = &t.getBodyQueryParam().get();
      break;
    case BodyProcessorType::MultiPart:
      body_query_params = &t.getBodyMultiPart().getNameValueLinked();
      body_query_params_map = &t.getBodyMultiPart().getNameValue();
      break;
    case BodyProcessorType::Json:
      body_query_params = &t.getBodyJson().getKeyValuesLinked();
      body_query_params_map = &t.getBodyJson().getKeyValues();
      break;
    default:
      body_query_params = &t.getBodyQueryParam().getLinked();
      body_query_params_map = &t.getBodyQueryParam().get();
      break;
    }

    RETURN_IF_COUNTER(
        // collection
        { result.append(static_cast<int>(line_query_params.size() + body_query_params->size())); },
        // specify subname
        {
          int count = line_query_params_map.count(sub_name_);
          count += body_query_params_map->count(sub_name_);
          result.append(count);
        });

    RETURN_VALUE(
        // collection
        {
          for (auto& elem : line_query_params) {
            if (!hasExceptVariable(elem.first)) [[likely]] {
              result.append(elem.first, elem.first);
            }
          }
          for (auto& elem : *body_query_params) {
            if (!hasExceptVariable(elem.first)) [[likely]] {
              result.append(elem.first, elem.first);
            }
          }
        },
        // collection regex
        {
          for (auto& elem : line_query_params) {
            if (!hasExceptVariable(elem.first)) [[likely]] {
              if (match(elem.first)) {
                result.append(elem.first, elem.first);
              }
            }
          }
          for (auto& elem : *body_query_params) {
            if (!hasExceptVariable(elem.first)) [[likely]] {
              if (match(elem.first)) {
                result.append(elem.first, elem.first);
              }
            }
          }
        },
        // specify subname
        {
          auto range = line_query_params_map.equal_range(sub_name_);
          for (auto iter = range.first; iter != range.second; ++iter) {
            result.append(iter->first);
          }
          auto range2 = body_query_params_map->equal_range(sub_name_);
          for (auto iter = range2.first; iter != range2.second; ++iter) {
            result.append(iter->first);
          }
        });
  }

  bool isCollection() const override { return sub_name_.empty(); };
};
} // namespace Variable
} // namespace Wge