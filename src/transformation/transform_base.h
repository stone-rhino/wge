#pragma once

#include <string>

namespace SrSecurity {
namespace Transformation {
class TransformBase {
public:
  /**
   * Evaluate the transformation.
   * @param data the data to be transformed.
   * @param data_len the length of the data.
   * @return the transformed data. if the transformation fails, return an empty string.
   */
  virtual std::string evaluate(const void* data, size_t data_len) const = 0;
};
} // namespace Transformation
} // namespace SrSecurity
