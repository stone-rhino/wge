#pragma once

#include "collection_base.h"
#include "variable_base.h"

namespace SrSecurity {
namespace Variable {
class MatchedVarsNames : public VariableBase, public CollectionBase {
  DECLARE_VIRABLE_NAME(MATCHED_VARS_NAMES);

public:
  MatchedVarsNames(std::string&& sub_name, bool is_not, bool is_counter)
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
          for (auto& [variable, variable_value] : t.getMatchedVariables()) {
            auto full_name = variable->fullName();
            if (!hasExceptVariable(full_name.sub_name_)) [[likely]] {
              if (variable->isCollection()) {
                result.append(
                    std::format("{}:{}", variable->mainName(), variable_value.variable_sub_name_));
              } else {
                result.append(full_name.tostring());
              }
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