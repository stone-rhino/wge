#pragma once

#include "args_get_names.h"
#include "args_post_names.h"
#include "collection_base.h"
#include "variable_base.h"

namespace SrSecurity {
namespace Variable {
class ArgsNames : public VariableBase, public CollectionBase {
  DECLARE_VIRABLE_NAME(ARGS_NAMES);

public:
  ArgsNames(std::string&& sub_name, bool is_not, bool is_counter)
      : VariableBase(std::move(sub_name), is_not, is_counter), CollectionBase(sub_name_) {}

public:
  void evaluate(Transaction& t, Common::EvaluateResults& result) const override {
    ArgsGetNames args_get_names(std::string(sub_name_), is_not_, is_counter_);
    args_get_names.evaluate(t, result);
    ArgsPostNames args_post_names(std::string(sub_name_), is_not_, is_counter_);
    args_post_names.evaluate(t, result);
  };

  bool isCollection() const override { return sub_name_.empty(); };
};
} // namespace Variable
} // namespace SrSecurity