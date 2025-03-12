#pragma once

#include <string>

#include "../common/variant.h"
#include "../macro/macro_base.h"
#include "../transaction.h"

#define DECLARE_OPERATOR_NAME(n)                                                                   \
public:                                                                                            \
  const char* name() const override { return name_; }                                              \
                                                                                                   \
private:                                                                                           \
  static constexpr char name_[] = #n;

#define MACRO_EXPAND_STRING_VIEW(var)                                                              \
  Common::EvaluateResult result;                                                                   \
  macro_->evaluate(t, result);                                                                     \
  std::string_view var = std::get<std::string_view>(result.front());

#define MACRO_EXPAND_INT(var)                                                                      \
  Common::EvaluateResult result;                                                                   \
  macro_->evaluate(t, result);                                                                     \
  int var = std::get<int>(result.front());

namespace SrSecurity {
namespace Operator {
/**
 * Base class for all operators.
 */
class OperatorBase {
public:
  OperatorBase(std::string&& literal_value, bool is_not)
      : literal_value_(std::move(literal_value)), is_not_(is_not) {}

  OperatorBase(const std::shared_ptr<Macro::MacroBase> macro, bool is_not)
      : macro_(macro), is_not_(is_not) {}

  virtual ~OperatorBase() = default;

public:
  /**
   * Get the literal value of the operator.
   * @return the literal value of the operator.
   */
  const std::string& literalValue() const { return literal_value_; }

  /**
   * Get the macro of the operator.
   * @return the macro of the operator.
   */
  const std::shared_ptr<Macro::MacroBase> macro() const { return macro_; }

public:
  /**
   * Evaluate the operator.
   * @param t the transaction.
   * @param operand the operand to evaluate.
   * @return true if the value matches the operator, false otherwise.
   */
  virtual bool evaluate(Transaction& t, const Common::Variant& operand) const = 0;

  /**
   * Get the name of the operator.
   * @return the name of the operator.
   */
  virtual const char* name() const = 0;

protected:
  std::string literal_value_;
  bool is_not_;
  const std::shared_ptr<Macro::MacroBase> macro_;
};

} // namespace Operator
} // namespace SrSecurity