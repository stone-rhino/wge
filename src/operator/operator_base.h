#pragma once

#include <string>
#include <string_view>

#include "../transaction.h"

#define DECLARE_OPERATOR_NAME(n)                                                                   \
public:                                                                                            \
  const char* name() const override { return name_; }                                              \
                                                                                                   \
private:                                                                                           \
  static constexpr char name_[] = #n;

namespace SrSecurity {
namespace Operator {
/**
 * Base class for all operators.
 */
class OperatorBase {
public:
  OperatorBase(std::string&& literal_value) : literal_value_(std::move(literal_value)) {}
  virtual ~OperatorBase() = default;

public:
  /**
   * Get the literal value of the operator.
   * @return the literal value of the operator.
   */
  const std::string& literalValue() const { return literal_value_; }

public:
  /**
   * Evaluate the operator.
   * @param t the transaction.
   * @param operand the operand to evaluate.
   * @return true if the value matches the operator, false otherwise.
   */
  virtual bool evaluate(Transaction& t, std::string_view operand) const = 0;

  /**
   * Get the name of the operator.
   * @return the name of the operator.
   */
  virtual const char* name() const = 0;

protected:
  std::string literal_value_;
};

} // namespace Operator
} // namespace SrSecurity