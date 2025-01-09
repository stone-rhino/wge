#pragma once

#include "macro_base.h"

namespace SrSecurity {
namespace Macro {
class Tx : public MacroBase {
public:
  Tx(std::string&& value) : MacroBase(std::move(value)) {
    assert(macro_name_.starts_with("tx."));
    variable_name_ = macro_name_.substr(3);
  }

public:
  std::string* evaluate(Transaction* t) override { return t->getVariable(variable_name_); }

private:
  std::string variable_name_;
};
} // namespace Macro
} // namespace SrSecurity