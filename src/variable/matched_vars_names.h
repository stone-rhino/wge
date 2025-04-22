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
class MatchedVarsNames : public VariableBase, public CollectionBase {
  DECLARE_VIRABLE_NAME(MATCHED_VARS_NAMES);

public:
  MatchedVarsNames(std::string&& sub_name, bool is_not, bool is_counter)
      : VariableBase(std::move(sub_name), is_not, is_counter), CollectionBase(sub_name_) {}

public:
  void evaluate(Transaction& t, Common::EvaluateResults& result) const override {
    RETURN_IF_COUNTER(
        // collection
        { result.append(static_cast<int>(t.getMatchedVariables().size())); },
        // specify subname
        { UNREACHABLE(); });

    RETURN_VALUE(
        // collection
        {
          for (auto& [variable, variable_value] : t.getMatchedVariables()) {
            auto full_name = variable->fullName();
            if (!hasExceptVariable(full_name.sub_name_)) [[likely]] {
              if (variable->isCollection()) {
                result.append(
                    std::format("{}:{}", variable->mainName(), variable_value.variable_sub_name_));
              } else {
                result.append(full_name.tostring());
              }
            }
          }
        },
        // collection regex
        { UNREACHABLE(); },
        // specify subname
        { UNREACHABLE(); });
  };

  bool isCollection() const override { return sub_name_.empty(); };
};
} // namespace Variable
} // namespace Wge