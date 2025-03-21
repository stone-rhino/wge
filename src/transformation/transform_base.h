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
   * @return the transformed data. if the transformation is failed, return an empty string.
   */
  virtual std::string evaluate(std::string_view data) const = 0;
};
} // namespace Transformation
} // namespace SrSecurity
