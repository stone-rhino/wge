#pragma once

#include "collection_base.h"
#include "variable_base.h"

namespace SrSecurity {
namespace Variable {
class MatchedVars : public VariableBase, public CollectionBase {
  DECLARE_VIRABLE_NAME(MATCHED_VARS);

public:
  MatchedVars(std::string&& sub_name, bool is_not, bool is_counter)
      : VariableBase(std::move(sub_name), is_not, is_counter), CollectionBase(sub_name_) {}

public:
  void evaluate(Transaction& t, Common::EvaluateResults& result) const override {
    RETURN_IF_COUNTER(
        // collection
        { result.append(static_cast<int>(t.getMatchedVariables().size())); },
        // specify subname
        { UNREACHABLE(); });

    RETURN_VALUE(
        // collection
        {
          for (auto& [variable, value] : t.getMatchedVariables()) {
            auto full_name = variable->fullName();
            if (!hasExceptVariable(full_name.sub_name_)) [[likely]] {
              result.append(value.variant_);
            }
          }
        },
        // collection regex
        { UNREACHABLE(); },
        // specify subname
        { UNREACHABLE(); });
  };

  bool isCollection() const override { return sub_name_.empty(); };
};
} // namespace Variable
} // namespace SrSecurity