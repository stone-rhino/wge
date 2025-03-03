#pragma once

#include "macro_base.h"

namespace SrSecurity {
namespace Macro {
class ReqbodyErrorMsg : public MacroBase {
public:
  const Common::Variant& evaluate(Transaction& t) override {
    UNREACHABLE();
    throw "Not implemented!";
  }
};
} // namespace Macro
} // namespace SrSecurity