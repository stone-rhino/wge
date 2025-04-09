#pragma once

#include <string>

#include "transform_base.h"

namespace SrSecurity {
namespace Transformation {
class ParityEven7Bit : public TransformBase {
  DECLARE_TRANSFORM_NAME(parityEven7Bit);

public:
  bool evaluate(std::string_view data, std::string& result) const override {
    assert(false);
    throw "Not implemted!";
  }
};
} // namespace Transformation
} // namespace SrSecurity
