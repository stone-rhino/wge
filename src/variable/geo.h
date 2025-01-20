#pragma once

#include "variable_base.h"

namespace SrSecurity {
namespace Variable {
class Geo : public VariableBase {
  DECLARE_VIRABLE_NAME(GEO);

public:
  Geo(std::string&& sub_name, bool is_not, bool is_counter)
      : VariableBase(std::move(sub_name), is_not, is_counter) {}

public:
  void preCompile() override {}
};
} // namespace Variable
} // namespace SrSecurity