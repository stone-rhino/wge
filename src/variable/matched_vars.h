#pragma once

#include "collection_base.h"
#include "variable_base.h"

namespace SrSecurity {
namespace Variable {
class MatchedVars : public VariableBase, public CollectionBase {
  DECLARE_VIRABLE_NAME(MATCHED_VARS);

public:
  MatchedVars(std::string&& sub_name, bool is_not, bool is_counter)
      : VariableBase(std::move(sub_name), is_not, is_counter) {}

public:
  void evaluate(Transaction& t, Common::EvaluateResults& result) const override {
    assert(!t.getMatchedVariables().empty());
    if (!t.getMatchedVariables().empty()) [[likely]] {
      if (!is_counter_) [[likely]] {
        for (auto& [variable, value] : t.getMatchedVariables()) {
          auto full_name = variable->fullName();
          if (!hasExceptVariable(full_name.sub_name_)) [[likely]] {
            result.append(value.variant_);
          }
        }
      } else {
        result.append(static_cast<int>(t.getMatchedVariables().size()));
      }
    }
  };

  bool isCollection() const override { return sub_name_.empty(); };
};
} // namespace Variable
} // namespace SrSecurity