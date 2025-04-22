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

#include "../common/evaluate_result.h"
#include "../transaction.h"

#define DECLARE_TRANSFORM_NAME(n)                                                                  \
public:                                                                                            \
  const char* name() const override { return name_; }                                              \
                                                                                                   \
private:                                                                                           \
  static constexpr char name_[] = #n;

namespace SrSecurity {
namespace Transformation {
class TransformBase {
public:
  virtual ~TransformBase() = default;

public:
  /**
   * Evaluate the transformation.
   * The different between this method and another overloaded evaluate method in the protected
   * section is that this method will check the cache first, then call the protected method to
   * evaluate the transformation. As a result, this method avoids the duplicated evaluation of the
   * same transformation.
   * @param t the reference to the transaction.
   * @param variable the pointer to the variable.
   * @param data the reference to the data to be transformed, and the transformed data will be
   * stored in it.
   * @return ture if the transformation is successful, otherwise false the data will not be
   * modified.
   */
  bool evaluate(Transaction& t, const Variable::VariableBase* variable,
                Common::EvaluateResults::Element& data) const;

  /**
   * Get the name of the transform.
   * @return the mname of the transform.
   */
  virtual const char* name() const = 0;

protected:
  /**
   * Evaluate the transformation.
   * @param data the data to be transformed.
   * @param result the reference to the transformed data.
   * @return true if the transformation is successful, otherwise false and the result will be empty.
   */
  virtual bool evaluate(std::string_view data, std::string& result) const = 0;

  /**
   * Check if the transformation needs to be converted to int.
   * @return true if the transformation needs to be converted to int, otherwise false.
   */
  virtual bool convertToInt() const { return false; }
};
} // namespace Transformation
} // namespace SrSecurity
