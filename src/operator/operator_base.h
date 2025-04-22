/**
 * Copyright (c) 2024-2025 Stone Rhino and contributors.
 *
 * MIT License (http://opensource.org/licenses/MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
 * associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
 * NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
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
  Common::EvaluateResults result;                                                                  \
  macro_->evaluate(t, result);                                                                     \
  std::string_view var = std::get<std::string_view>(result.front().variant_);

#define MACRO_EXPAND_INT(var)                                                                      \
  Common::EvaluateResults result;                                                                  \
  macro_->evaluate(t, result);                                                                     \
  int var = std::get<int>(result.front().variant_);

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

  /**
   * Check if the operator is a NOT operator.
   * @return true if the operator is a NOT operator, false otherwise.
   */
  bool isNot() const { return is_not_; }

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