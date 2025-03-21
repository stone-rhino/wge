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
  void evaluate(Transaction& t, Common::EvaluateResults& result) const override {
    assert(!t.getMatchedVariables().empty());
    if (!t.getMatchedVariables().empty()) [[likely]] {
      if (!is_counter_) [[likely]] {
        auto& matched_var = t.getMatchedVariables().back();
        if (matched_var.first->isCollection()) [[likely]] {
          result.append(std::format("{}:{}", matched_var.first->mainName(),
                                    matched_var.second.variable_sub_name_));
        } else {
          result.append(matched_var.first->fullName().tostring());
        }
      } else {
        result.append(1);
      }
    }
  };
};
} // namespace Variable
} // namespace SrSecurity