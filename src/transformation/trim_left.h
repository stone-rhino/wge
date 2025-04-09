#pragma once

#include <string>

#include "transform_base.h"

namespace SrSecurity {
namespace Transformation {
class TrimLeft : public TransformBase {
  DECLARE_TRANSFORM_NAME(trimLeft);

public:
  bool evaluate(std::string_view data, std::string& result) const override {
    assert(false);
    throw "Not implemted!";
  }
};
} // namespace Transformation
} // namespace SrSecurity
