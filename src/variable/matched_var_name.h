#pragma once

#include "variable_base.h"

namespace SrSecurity {
namespace Variable {
class MatchedVarName : public VariableBase {
  DECLARE_VIRABLE_NAME(MATCHED_VAR_NAME);

public:
  MatchedVarName(std::string&& sub_name, bool is_not, bool is_counter)
      : VariableBase(std::move(sub_name), is_not, is_counter) {}

public:
  void evaluate(Transaction& t, Common::EvaluateResult& result) const override {
    assert(t.getCurrentVariable());
    if (!is_counter_) [[likely]] {
      result.append(t.getCurrentVariable()->fullName().tostring());
    } else {
      result.append(t.getCurrentVariable() ? 1 : 0);
    }
  };
};
} // namespace Variable
} // namespace SrSecurity