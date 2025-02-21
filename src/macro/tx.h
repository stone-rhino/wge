#pragma once

#include "macro_base.h"

#include "../common/log.h"

namespace SrSecurity {
namespace Macro {
class Tx : public MacroBase {
public:
  Tx(std::string&& variable_name) : variable_name_(std::move(variable_name)) {}

public:
  const Common::Variant& evaluate(Transaction& t) override {
    SRSECURITY_LOG_TRACE("macro %{{TX.{}}} expanded: {}", variable_name_,
                         VISTIT_VARIANT_AS_STRING(t.getVariable(variable_name_)));
    return t.getVariable(variable_name_);
  }

private:
  std::string variable_name_;
};
} // namespace Macro
} // namespace SrSecurity