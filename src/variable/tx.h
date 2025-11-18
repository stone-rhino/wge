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

#include <optional>

#include "collection_base.h"
#include "evaluate_help.h"
#include "variable_base.h"

namespace Wge {
namespace Variable {
class Tx final : public VariableBase, public CollectionBase {
  DECLARE_VIRABLE_NAME(TX);

public:
  Tx(std::string&& sub_name, std::optional<size_t> index, bool is_not, bool is_counter,
     std::string_view curr_rule_file_path)
      : VariableBase(std::move(sub_name), is_not, is_counter),
        CollectionBase(sub_name_, curr_rule_file_path), index_(index) {
    if (!sub_name_.empty() && std::all_of(sub_name_.begin(), sub_name_.end(), ::isdigit)) {
      capture_index_ = ::atoi(sub_name_.c_str());
    }
  }

public:
  void evaluate(Transaction& t, Common::EvaluateResults& result) const override {
    // Process capture that definded by TX:[1-99]
    if (capture_index_.has_value())
      [[unlikely]] {
        if (is_counter_)
          [[unlikely]] {
            result.emplace_back(t.getCapture(capture_index_.value()).empty() ? 0 : 1);
          }
        else {
          result.emplace_back(t.getCapture(capture_index_.value()));
        }
        return;
      }

    // Process single variable and collection
    RETURN_IF_COUNTER(
        // collection
        { result.emplace_back(t.getVariablesCount()); },
        // specify subname
        {
          if (index_.has_value())
            [[likely]] {
              t.hasVariable(index_.value()) ? result.emplace_back(1) : result.emplace_back(0);
            }
          else {
            t.hasVariable(sub_name_) ? result.emplace_back(1) : result.emplace_back(0);
          }
        });

    RETURN_VALUE(
        // collection
        {
          auto variables = t.getVariables();
          for (auto variable : variables) {
            if (!hasExceptVariable(t, main_name_, variable.first))
              [[likely]] { result.emplace_back(*variable.second, variable.first); }
          }
        },
        // collection regex
        {
          auto variables = t.getVariables();
          for (auto variable : variables) {
            if (!hasExceptVariable(t, main_name_, variable.first))
              [[likely]] {
                if (match(variable.first)) {
                  result.emplace_back(*variable.second, variable.first);
                }
              }
          }
        },
        // specify subname
        {
          if (index_.has_value())
            [[likely]] { result.emplace_back(t.getVariable(index_.value())); }
          else {
            result.emplace_back(t.getVariable(sub_name_));
          }
        });
  }

  bool isCollection() const override { return sub_name_.empty(); };

private:
  std::optional<size_t> index_;
  std::optional<size_t> capture_index_;
};
} // namespace Variable
} // namespace Wge