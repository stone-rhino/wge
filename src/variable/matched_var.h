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
  void evaluate(Transaction& t, Common::EvaluateResult& result) const override {
    assert(!t.getMatchedVariables().empty());
    if (!t.getMatchedVariables().empty()) [[likely]] {
      if (!is_counter_) [[likely]] {
        result.append(t.getMatchedVariables().back().second.variant_);
      } else {
        result.append(1);
      }
    }
  };
};
} // namespace Variable
} // namespace SrSecurity