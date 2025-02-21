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
  MultiMacro(std::string&& variable_name, std::vector<std::shared_ptr<MacroBase>>&& macros)
      : variable_name_(std::move(variable_name)), macros_(std::move(macros)) {}

public:
  const Common::Variant& evaluate(Transaction& t) override {
    std::string ev = variable_name_;
    for (auto& macro : macros_) {
      auto pos1 = ev.find("%{");
      assert(pos1 != std::string::npos);
      if (pos1 != ev.npos) {
        auto pos2 = ev.find('}', pos1);
        assert(pos2 != std::string::npos);
        auto& mv = macro->evaluate(t);
        if (IS_INT_VARIANT(mv)) {
          ev = ev.replace(pos1, pos2 - pos1 + 1, std::to_string(std::get<int>(mv)));
        } else if (IS_STRING_VARIANT(mv)) {
          ev = ev.replace(pos1, pos2 - pos1 + 1, std::get<std::string>(mv));
        } else if (IS_STRING_VIEW_VARIANT(mv)) {
          auto& sv = std::get<std::string_view>(mv);
          ev = ev.replace(pos1, pos2 - pos1 + 1, sv.data(), sv.size());
        } else [[unlikely]] {
          UNREACHABLE();
          ev = ev.replace(pos1, pos2 - pos1 + 1, "");
        }
      }
    }
    evaluate_value_ = ev;

    SRSECURITY_LOG_TRACE("macro {} expanded: {}", variable_name_,
                         VISTIT_VARIANT_AS_STRING(evaluate_value_));

    return evaluate_value_;
  }

private:
  std::string variable_name_;
  std::vector<std::shared_ptr<MacroBase>> macros_;
};
} // namespace Macro
} // namespace SrSecurity