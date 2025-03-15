#pragma once

#include "variable_base.h"

namespace SrSecurity {
namespace Variable {
class MatchedVars : public VariableBase {
  DECLARE_VIRABLE_NAME(MATCHED_VARS);

public:
  MatchedVars(std::string&& sub_name, bool is_not, bool is_counter)
      : VariableBase(std::move(sub_name), is_not, is_counter) {}

public:
  void evaluate(Transaction& t, Common::EvaluateResult& result) const override {
    assert(!t.getMatchedVariables().empty());
    if (!t.getMatchedVariables().empty()) [[likely]] {
      if (!is_counter_) [[likely]] {
        for (auto& [_, value] : t.getMatchedVariables()) {
          result.append(value.variant_);
        }
      } else {
        result.append(static_cast<int>(t.getMatchedVariables().size()));
      }
    }
  };
};
} // namespace Variable
} // namespace SrSecurity