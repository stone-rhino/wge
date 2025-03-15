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
    assert(!IS_EMPTY_VARIANT(t.getCurrentVariableResult().variant_));
    if (!is_counter_) [[likely]] {
      result.append(t.getCurrentVariableResult().variant_);
    } else {
      result.append(IS_EMPTY_VARIANT(t.getCurrentVariableResult().variant_) ? 0 : 1);
    }
  };
};
} // namespace Variable
} // namespace SrSecurity