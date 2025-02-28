#pragma once

#include "macro_base.h"

namespace SrSecurity {
namespace Macro {
class Session : public MacroBase {
public:
  const Common::Variant& evaluate(Transaction& t) {
    UNREACHABLE();
    throw "Not implemented!";
  }
};
} // namespace Macro
} // namespace SrSecurity