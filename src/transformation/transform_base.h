#pragma once

#include <string>

namespace SrSecurity {
namespace Transformation {
class TransformBase {
public:
  virtual void evaluate(const void* data, size_t data_len, void* buffer, size_t buffer_len) = 0;
};
} // namespace Transformation
} // namespace SrSecurity
