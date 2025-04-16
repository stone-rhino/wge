#pragma once

#include <string>

#include "transform_base.h"

namespace SrSecurity {
namespace Transformation {
class Length : public TransformBase {
  DECLARE_TRANSFORM_NAME(length);

public:
  bool evaluate(std::string_view data, std::string& result) const override {
    result = std::to_string(data.length());
    return true;
  }

  bool convertToInt() const override { return true; }
};
} // namespace Transformation
} // namespace SrSecurity
