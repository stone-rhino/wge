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
  const Common::Variant& evaluate(Transaction& t) const override {
    auto& buffer = t.evaluatedBuffer().variable_;
    buffer = t.getConnectionInfo().downstream_ip_;
    return buffer;
  };
};
} // namespace Variable
} // namespace SrSecurity