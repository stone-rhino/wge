#pragma once

#include "variable_base.h"

namespace SrSecurity {
namespace Variable {
class Tx : public VariableBase {
public:
  Tx(std::string&& full_name, bool is_not, bool is_counter)
      : VariableBase(std::move(full_name), is_not, is_counter) {}

public:
  void preCompile() override {}
};
} // namespace Variable
} // namespace SrSecurity