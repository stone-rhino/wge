#pragma once

#include "macro_base.h"

namespace SrSecurity {
namespace Macro {
class Tx : public MacroBase {
public:
  Tx(std::string&& variable_name) : variable_name_(std::move(variable_name)) {}

public:
  std::string_view evaluate(Transaction& t) override { return t.getVariable(variable_name_); }

private:
  std::string variable_name_;
};
} // namespace Macro
} // namespace SrSecurity