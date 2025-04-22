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

#include <string>
#include <string_view>

#include "collection_base.h"
#include "variable_base.h"

namespace SrSecurity {
namespace Variable {
class ArgsPost : public VariableBase, public CollectionBase {
  friend class Args;
  DECLARE_VIRABLE_NAME(ARGS_POST);

public:
  ArgsPost(std::string&& sub_name, bool is_not, bool is_counter)
      : VariableBase(std::move(sub_name), is_not, is_counter), CollectionBase(sub_name_) {}

public:
  void evaluate(Transaction& t, Common::EvaluateResults& result) const override {
    // Get the query params by the request body processor type
    const std::vector<std::unordered_multimap<std::string_view, std::string_view>::iterator>*
        query_params = nullptr;
    const std::unordered_multimap<std::string_view, std::string_view>* query_params_map = nullptr;
    switch (t.getRequestBodyProcessor()) {
    case BodyProcessorType::UrlEncoded:
      query_params = &t.getBodyQueryParam().getLinked();
      query_params_map = &t.getBodyQueryParam().get();
      break;
    case BodyProcessorType::MultiPart:
      query_params = &t.getBodyMultiPart().getNameValueLinked();
      query_params_map = &t.getBodyMultiPart().getNameValue();
      break;
    default:
      query_params = &t.getBodyQueryParam().getLinked();
      query_params_map = &t.getBodyQueryParam().get();
      break;
    }

    RETURN_IF_COUNTER(
        // collection
        { result.append(static_cast<int>(query_params->size())); },
        // specify subname
        {
          auto iter = query_params_map->find(sub_name_);
          result.append(iter != query_params_map->end() ? 1 : 0);
        });

    RETURN_VALUE(
        // collection
        {
          for (auto& elem : *query_params) {
            if (!hasExceptVariable(elem->first)) [[likely]] {
              result.append(elem->second, elem->first);
            }
          }
        },
        // collection regex
        {
          for (auto& elem : *query_params) {
            if (!hasExceptVariable(elem->first)) [[likely]] {
              if (match(elem->first)) {
                result.append(elem->second, elem->first);
              }
            }
          }
        },
        // specify subname
        {
          if (!hasExceptVariable(sub_name_)) [[likely]] {
            auto iter = query_params_map->find(sub_name_);
            if (iter != query_params_map->end()) {
              result.append(iter->second);
            }
          }
        });
  };

  bool isCollection() const override { return sub_name_.empty(); };
};
} // namespace Variable
} // namespace SrSecurity