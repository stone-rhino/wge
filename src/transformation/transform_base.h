#pragma once

#include <string>

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
   * @param data the data to be transformed.
   * @param data_len the length of the data.
   * @return the transformed data. if the transformation fails, return an empty string.
   */
  virtual std::string evaluate(const void* data, size_t data_len) const = 0;

  /**
   * Get the name of the transform.
   * @return the mname of the transform.
   */
  virtual const char* name() const = 0;
};
} // namespace Transformation
} // namespace SrSecurity
