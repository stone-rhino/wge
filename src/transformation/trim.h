#pragma once

#include <string>

#include "transform_base.h"

namespace SrSecurity {
namespace Transformation {
class Trim : public TransformBase {
  DECLARE_TRANSFORM_NAME(trim);

public:
  bool evaluate(std::string_view data, std::string& result) const override {
    assert(false);
    throw "Not implemted!";
  }
};
} // namespace Transformation
} // namespace SrSecurity
