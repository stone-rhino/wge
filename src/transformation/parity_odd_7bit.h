#pragma once

#include <string>

#include "transform_base.h"

namespace SrSecurity {
namespace Transformation {
class ParityOdd7Bit : public TransformBase {
  DECLARE_TRANSFORM_NAME(parityOdd7Bit);

public:
  std::string evaluate(std::string_view data) const override {
    assert(false);
    throw "Not implemted!";
  }
};
} // namespace Transformation
} // namespace SrSecurity
