#pragma once

#include "variable_base.h"

namespace SrSecurity {
namespace Variable {
class RemoteAddr : public VariableBase {
  DECLARE_VIRABLE_NAME(REMOTE_ADDR);

public:
  RemoteAddr(std::string&& sub_name, bool is_not, bool is_counter)
      : VariableBase(std::move(sub_name), is_not, is_counter) {}

public:
  void evaluate(Transaction& t, Common::EvaluateResult& result) const override {
    if (!is_counter_) [[likely]] {
      result.set(t.getConnectionInfo().downstream_ip_);
    } else {
      result.set(t.getConnectionInfo().downstream_ip_.empty() ? 0 : 1);
    }
  };
};
} // namespace Variable
} // namespace SrSecurity