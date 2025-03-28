#pragma once

#include "variable_base.h"

namespace SrSecurity {
namespace Variable {
class MatchedVar : public VariableBase {
  DECLARE_VIRABLE_NAME(MATCHED_VAR);

public:
  MatchedVar(std::string&& sub_name, bool is_not, bool is_counter)
      : VariableBase(std::move(sub_name), is_not, is_counter) {}

public:
  void evaluate(Transaction& t, Common::EvaluateResults& result) const override {
    if (is_counter_) [[unlikely]] {
      if (!t.getMatchedVariables().empty()) [[likely]] {
        result.append(1);
      } else {
        result.append(0);
      }
      return;
    }

    if (t.getMatchedVariables().empty()) [[unlikely]] {
      return;
    }

    auto& matched_var = t.getMatchedVariables().back();
    result.append(matched_var.second.variant_);
  };
};
} // namespace Variable
} // namespace SrSecurity