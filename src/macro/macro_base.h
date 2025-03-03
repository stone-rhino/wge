#pragma once

#include "../common/assert.h"
#include "../common/variant.h"
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
  /**
   * Evaluate the macro.
   * @param t the transaction.
   * @return the value of the macro.
   * @note The result of the macro evaluation is stored in the transaction's evaluated buffer. In
   * each transaction, all macros share the same evaluated buffer, so we need to copy it to a
   * local variable if we want to use it after the next macro object is evaluated.
   */
  virtual const Common::Variant& evaluate(Transaction& t) = 0;
};
} // namespace Macro
} // namespace SrSecurity