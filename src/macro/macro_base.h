#pragma once

#include <string>

#include "../transaction.h"

namespace SrSecurity {
namespace Macro {
/**
 * Macros allow for using place holders in rules that will be expanded out to their values at
 * runtime. Currently only variable expansion is supported, however more options may be added in
 * future versions of ModSecurity.
 * Format:
 * %{VARIABLE}
 * %{COLLECTION.VARIABLE}
 */
class MacroBase {
public:
  MacroBase() {}

public:
  virtual std::string* evaluate(Transaction& t) = 0;
};
} // namespace Macro
} // namespace SrSecurity