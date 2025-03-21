#pragma once

#include <string>
#include <string_view>

#include "args_get.h"
#include "args_post.h"
#include "collection_base.h"
#include "variable_base.h"

namespace SrSecurity {
namespace Variable {
class Args : public VariableBase, public CollectionBase {
  DECLARE_VIRABLE_NAME(ARGS);

public:
  Args(std::string&& sub_name, bool is_not, bool is_counter)
      : VariableBase(std::move(sub_name), is_not, is_counter) {}

public:
  void evaluate(Transaction& t, Common::EvaluateResults& result) const override {
    assert(false);
    throw "Not implemented!";
  };
  bool isCollection() const override { return sub_name_.empty(); };
};
} // namespace Variable
} // namespace SrSecurity