#pragma once

#include <format>
#include <memory>
#include <thread>
#include <vector>

#include "macro_base.h"

#include "../common/assert.h"
#include "../common/log.h"
#include "../common/variant.h"

namespace SrSecurity {
namespace Macro {
class MultiMacro : public MacroBase {
public:
  MultiMacro(std::string&& literal_value, std::vector<std::shared_ptr<MacroBase>>&& macros)
      : MacroBase(std::move(literal_value)), macros_(std::move(macros)) {}

public:
  void evaluate(Transaction& t, Common::EvaluateResult& result) override {
    std::string eval = literal_value_;
    for (auto& macro : macros_) {
      auto pos1 = eval.find("%{");
      assert(pos1 != std::string::npos);
      if (pos1 != eval.npos) {
        auto pos2 = eval.find('}', pos1);
        assert(pos2 != std::string::npos);
        macro->evaluate(t, result);
        if (IS_INT_VARIANT(result.front())) {
          eval = eval.replace(pos1, pos2 - pos1 + 1, std::to_string(std::get<int>(result.front())));
        } else if (IS_STRING_VIEW_VARIANT(result.front())) {
          auto& sv = std::get<std::string_view>(result.front());
          eval = eval.replace(pos1, pos2 - pos1 + 1, sv.data(), sv.size());
        } else [[unlikely]] {
          UNREACHABLE();
          eval = eval.replace(pos1, pos2 - pos1 + 1, "");
        }

        // Clear the result for the next macro.
        result.clear();
      }
    }
    result.append(std::move(eval));
    assert(eval.empty());

    SRSECURITY_LOG_TRACE("macro {} expanded: {}", literal_value_,
                         VISTIT_VARIANT_AS_STRING(result.front()));
  }

private:
  std::vector<std::shared_ptr<MacroBase>> macros_;
};
} // namespace Macro
} // namespace SrSecurity