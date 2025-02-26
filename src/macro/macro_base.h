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
   * @note The result of the macro expansion is stored in a thread_local variable that all macro
   * objects share. So we need to copy it to a local variable if we want to use it after the next
   * macro object is evaluated.
   */
  virtual const Common::Variant& evaluate(Transaction& t) = 0;

protected:
  // The result of the macro expansion
  // All threads share the same rule object, that means all threads share the same macro object.
  // So we need to use thread_local to avoid.
  static thread_local Common::Variant evaluate_value_;
};
} // namespace Macro
} // namespace SrSecurity