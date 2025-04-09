#pragma once

#include <string>

#include "transform_base.h"

namespace SrSecurity {
namespace Transformation {
class Base64DecodeExt : public TransformBase {
  DECLARE_TRANSFORM_NAME(base64DecodeExt);

public:
  bool evaluate(std::string_view data, std::string& result) const override {
    assert(false);
    throw "Not implemted!";
  }
};
} // namespace Transformation
} // namespace SrSecurity
