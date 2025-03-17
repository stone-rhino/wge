#pragma once

#include "../common/assert.h"
#include "../common/evaluate_result.h"
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
  MacroBase(std::string&& literal_value) : literal_value_(std::move(literal_value)) {}
  virtual ~MacroBase() = default;

public:
  /**
   * Get the literal value of the macro.
   * @return the literal value of the macro.
   */
  const std::string& literalValue() const { return literal_value_; }

public:
  /**
   * Evaluate the macro.
   * @param t the transaction.
   * @param result the result of the evaluation.
   */
  virtual void evaluate(Transaction& t, Common::EvaluateResults& result) = 0;

protected:
  std::string literal_value_;
};
} // namespace Macro
} // namespace SrSecurity