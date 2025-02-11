#pragma once

#include <format>
#include <memory>
#include <vector>

#include "macro_base.h"

namespace SrSecurity {
namespace Macro {
class MultiMacro : public MacroBase {
public:
  MultiMacro(std::string&& var_name, std::vector<std::shared_ptr<MacroBase>>&& macros)
      : var_name_(std::move(var_name)), macros_(std::move(macros)) {}

public:
  std::string* evaluate(Transaction& t) override {
    evaluate_value_ = var_name_;
    for (auto& macro : macros_) {
      auto pos1 = evaluate_value_.find("%{");
      assert(pos1 != std::string::npos);
      if (pos1 != evaluate_value_.npos) {
        auto pos2 = evaluate_value_.find('}', pos1);
        assert(pos2 != std::string::npos);
        evaluate_value_ = evaluate_value_.replace(pos1, pos2 - pos1 + 1, *macro->evaluate(t));
      }
    }
    return &evaluate_value_;
  }

private:
  std::string var_name_;
  std::vector<std::shared_ptr<MacroBase>> macros_;
  std::string evaluate_value_;
};
} // namespace Macro
} // namespace SrSecurity