#pragma once

#include "variable_base.h"

#include "../common/empty_string.h"

namespace SrSecurity {
namespace Variable {
class Tx : public VariableBase {
  DECLARE_VIRABLE_NAME(TX);

public:
  Tx(std::string&& sub_name, bool is_not, bool is_counter)
      : VariableBase(std::move(sub_name), is_not, is_counter) {}

public:
  std::string_view evaluate(Transaction& t) const override {
    if (is_counter_) {
      return t.hasVariable(sub_name_) ? number_ : EMPTY_STRING_VIEW;
    } else {
      return t.getVariable(sub_name_);
    }
  }

private:
  static constexpr std::string_view number_{"1"};
};
} // namespace Variable
} // namespace SrSecurity