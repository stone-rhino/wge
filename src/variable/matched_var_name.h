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
    if (matched_var.first->isCollection()) [[likely]] {
      result.append(std::format("{}:{}", matched_var.first->mainName(),
                                matched_var.second.variable_sub_name_));
    } else {
      result.append(matched_var.first->fullName().tostring());
    }
  };
};
} // namespace Variable
} // namespace SrSecurity