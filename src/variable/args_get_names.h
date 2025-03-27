#pragma once

#include "collection_base.h"
#include "evaluate_help.h"
#include "variable_base.h"

namespace SrSecurity {
namespace Variable {
class ArgsGetNames : public VariableBase, public CollectionBase {
  DECLARE_VIRABLE_NAME(ARGS_GET_NAMES);

public:
  ArgsGetNames(std::string&& sub_name, bool is_not, bool is_counter)
      : VariableBase(std::move(sub_name), is_not, is_counter), CollectionBase(sub_name_) {}

public:
  void evaluate(Transaction& t, Common::EvaluateResults& result) const override {
    RETURN_IF_COUNTER(t.getRequestLineInfo().query_params_,
                      t.getRequestLineInfo().query_params_map_);

    RETURN_VALUE_FIRST(t.getRequestLineInfo().query_params_,
                       t.getRequestLineInfo().query_params_map_);
  };

  bool isCollection() const override { return sub_name_.empty(); };
};
} // namespace Variable
} // namespace SrSecurity