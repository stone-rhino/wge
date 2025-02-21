#pragma once

#include "variable_base.h"

namespace SrSecurity {
namespace Variable {
class Tx : public VariableBase {
  DECLARE_VIRABLE_NAME(TX);

public:
  Tx(std::string&& sub_name, bool is_not, bool is_counter)
      : VariableBase(std::move(sub_name), is_not, is_counter) {}

public:
  const Common::Variant& evaluate(Transaction& t) const override {
    const Common::Variant* variant;
    if (is_counter_) {
      return t.hasVariable(sub_name_) ? number_one_ : number_zero_;
    } else {
      return t.getVariable(sub_name_);
    }
  }

private:
  static constexpr Common::Variant number_zero_{0};
  static constexpr Common::Variant number_one_{1};
};
} // namespace Variable
} // namespace SrSecurity