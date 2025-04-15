#pragma once

#include "variable_base.h"

namespace SrSecurity {
namespace Variable {
class ResponseStatus : public VariableBase {
  DECLARE_VIRABLE_NAME(RESPONSE_STATUS);

public:
  ResponseStatus(std::string&& sub_name, bool is_not, bool is_counter)
      : VariableBase(std::move(sub_name), is_not, is_counter) {}

public:
  void evaluate(Transaction& t, Common::EvaluateResults& result) const override {
    if (is_counter_) [[unlikely]] {
      result.append(t.getResponseLineInfo().status_code_.empty() ? 0 : 1);
      return;
    }

    result.append(t.getResponseLineInfo().status_code_);
  };
};
} // namespace Variable
} // namespace SrSecurity