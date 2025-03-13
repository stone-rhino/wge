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
    assert(t.getCurrentVariableResult());
    if (!is_counter_) [[likely]] {
      result.append(*t.getCurrentVariableResult());
    } else {
      result.append(t.getCurrentVariableResult() ? 1 : 0);
    }
  };
};
} // namespace Variable
} // namespace SrSecurity