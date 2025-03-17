#pragma once

#include <string>

#include "transform_base.h"

namespace SrSecurity {
namespace Transformation {
class LowerCase : public TransformBase {
  DECLARE_TRANSFORM_NAME(lowercase);

public:
  std::string evaluate(std::string_view data) const override {
    std::string result;
    result.reserve(data.length());
    for (size_t i = 0; i < data.length(); ++i) {
      result.push_back(std::tolower(data[i]));
    }
    return result;
  }
};
} // namespace Transformation
} // namespace SrSecurity
