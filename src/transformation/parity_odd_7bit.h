#pragma once

#include <string>

#include "transform_base.h"

namespace SrSecurity {
namespace Transformation {
class ParityOdd7Bit : public TransformBase {
  DECLARE_TRANSFORM_NAME(parityOdd7Bit);

public:
  std::string evaluate(const void* data, size_t data_len) const override {
    assert(false);
    throw "Not implemted!";
  }
};
} // namespace Transformation
} // namespace SrSecurity
