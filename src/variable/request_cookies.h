#pragma once

#include "variable_base.h"

namespace SrSecurity {
namespace Variable {
class RequestCookies : public VariableBase {
  DECLARE_VIRABLE_NAME(REQUEST_COOKIES);

public:
  RequestCookies(std::string&& sub_name, bool is_not, bool is_counter)
      : VariableBase(std::move(sub_name), is_not, is_counter) {}

public:
  void preCompile() override {}
};
} // namespace Variable
} // namespace SrSecurity