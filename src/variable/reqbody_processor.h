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

#include <unordered_map>

#include "variable_base.h"

#include "../config.h"

namespace Wge {
namespace Variable {
class ReqBodyProcessor final : public VariableBase {
  DECLARE_VIRABLE_NAME(REQBODY_PROCESSOR);

public:
  ReqBodyProcessor(std::string&& sub_name, bool is_not, bool is_counter,
                   std::string_view curr_rule_file_path)
      : VariableBase(std::move(sub_name), is_not, is_counter) {}

public:
  void evaluate(Transaction& t, Common::EvaluateResults& result) const override {
    auto body_processor_type = t.getRequestBodyProcessor();
    auto iter = body_processor_type_map_.find(body_processor_type);

    if (is_counter_)
      [[unlikely]] {
        if (iter != body_processor_type_map_.end()) {
          result.emplace_back(1);
        } else {
          result.emplace_back(0);
        }
        return;
      }

    if (iter == body_processor_type_map_.end())
      [[unlikely]] { return; }

    result.emplace_back(iter->second);
  }

private:
  static const std::unordered_map<BodyProcessorType, std::string_view> body_processor_type_map_;
};
} // namespace Variable
} // namespace Wge