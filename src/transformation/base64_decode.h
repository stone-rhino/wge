#pragma once

#include <string>

#include "transform_base.h"

namespace SrSecurity {
namespace Transformation {
class Base64Decode : public TransformBase {
  DECLARE_TRANSFORM_NAME(base64Decode);

public:
  std::string evaluate(std::string_view data) const override {
    assert(false);
    throw "Not implemted!";
  }
};
} // namespace Transformation
} // namespace SrSecurity
