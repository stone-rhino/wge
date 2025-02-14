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
  const std::string& evaluate(Transaction& t) const override {
    std::string* value = t.getVariable(sub_name_);
    if (is_counter_) {
      if (value) {
        return number_;
      } else {
        return EMPTY_STRING;
      }
    } else {
      if (value) [[likely]] {
        return *value;
      } else {
        return EMPTY_STRING;
      }
    }
  }
  void preCompile() override {}

private:
  static constexpr std::string number_{"1"};
};
} // namespace Variable
} // namespace SrSecurity