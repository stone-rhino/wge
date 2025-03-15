#pragma once

#include "variable_base.h"

namespace SrSecurity {
namespace Variable {
class MatchedVarsNames : public VariableBase {
  DECLARE_VIRABLE_NAME(MATCHED_VARS_NAMES);

public:
  MatchedVarsNames(std::string&& sub_name, bool is_not, bool is_counter)
      : VariableBase(std::move(sub_name), is_not, is_counter) {}

public:
  void evaluate(Transaction& t, Common::EvaluateResult& result) const override {
    assert(!t.getMatchedVariables().empty());
    if (!t.getMatchedVariables().empty()) [[likely]] {
      if (!is_counter_) [[likely]] {
        for (auto& [variable, _] : t.getMatchedVariables()) {
          result.append(variable->fullName().tostring());
        }
      } else {
        result.append(static_cast<int>(t.getMatchedVariables().size()));
      }
    }
  };
};
} // namespace Variable
} // namespace SrSecurity