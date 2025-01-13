#pragma once

#include "macro_base.h"

namespace SrSecurity {
namespace Macro {
class Tx : public MacroBase {
public:
  Tx(std::string&& var_name) : var_name_(std::move(var_name)) {}

public:
  std::string* evaluate(Transaction& t) override { return t.getVariable(var_name_); }

private:
  std::string var_name_;
};
} // namespace Macro
} // namespace SrSecurity