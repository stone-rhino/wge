#pragma once

#include "variable_base.h"

namespace SrSecurity {
namespace Variable {
class RequestLine : public VariableBase {
  DECLARE_VIRABLE_NAME(REQUEST_LINE);

public:
  RequestLine(std::string&& sub_name, bool is_not, bool is_counter)
      : VariableBase(std::move(sub_name), is_not, is_counter) {}

public:
  void evaluate(Transaction& t, Common::EvaluateResult& result) const override {
    if (!is_counter_) [[likely]] {
      result.append(t.getUri());
    } else {
      result.append(t.getUri().empty() ? 0 : 1);
    }
  };
};
} // namespace Variable
} // namespace SrSecurity