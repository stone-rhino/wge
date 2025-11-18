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

#include <format>
#include <memory>
#include <thread>
#include <vector>

#include "macro_base.h"

#include "../common/assert.h"
#include "../common/log.h"
#include "../common/variant.h"

namespace Wge {
namespace Macro {
class MultiMacro final : public MacroBase {
public:
  MultiMacro(std::string&& literal_value, std::vector<std::unique_ptr<MacroBase>>&& macros)
      : MacroBase(std::move(literal_value)), macros_(std::move(macros)) {}

public:
  void evaluate(Transaction& t, Common::EvaluateResults& result) const override {
    std::string eval = literal_value_;
    for (auto& macro : macros_) {
      auto pos1 = eval.find("%{");
      assert(pos1 != std::string::npos);
      if (pos1 != eval.npos) {
        auto pos2 = eval.find('}', pos1);
        assert(pos2 != std::string::npos);
        macro->evaluate(t, result);
        if (IS_INT_VARIANT(result.front().variant_)) {
          eval = eval.replace(pos1, pos2 - pos1 + 1,
                              std::to_string(std::get<int64_t>(result.front().variant_)));
        } else if (IS_STRING_VIEW_VARIANT(result.front().variant_)) {
          std::string_view sv = std::get<std::string_view>(result.front().variant_);
          eval = eval.replace(pos1, pos2 - pos1 + 1, sv.data(), sv.size());
        } else
          [[unlikely]] {
            // UNREACHABLE();
            WGE_LOG_WARN(
                "macro {} expanded: unexpected variant type {}, expected int64_t or string_view",
                literal_value_, VISTIT_VARIANT_AS_STRING(result.front().variant_));
            eval = eval.replace(pos1, pos2 - pos1 + 1, "");
          }

        // Clear the result for the next macro.
        result.clear();
      }
    }
    result.emplace_back(t.internString(std::move(eval)));
    assert(eval.empty());

    WGE_LOG_TRACE("macro {} expanded: {}", literal_value_,
                  VISTIT_VARIANT_AS_STRING(result.front().variant_));
  }

private:
  std::vector<std::unique_ptr<MacroBase>> macros_;
};
} // namespace Macro
} // namespace Wge