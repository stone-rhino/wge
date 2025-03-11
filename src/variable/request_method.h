#pragma once

#include "variable_base.h"

namespace SrSecurity {
namespace Variable {
class RequestMothod : public VariableBase {
  DECLARE_VIRABLE_NAME(REQUEST_METHOD);

public:
  RequestMothod(std::string&& sub_name, bool is_not, bool is_counter)
      : VariableBase(std::move(sub_name), is_not, is_counter) {}

public:
  void evaluate(Transaction& t, Common::EvaluateResult& result) const override {
    if (!is_counter_) [[likely]] {
      result.set(t.getUriInfo().method_);
    } else {
      result.set(t.getUriInfo().method_.empty() ? 0 : 1);
    }
  };
};
} // namespace Variable
} // namespace SrSecurity