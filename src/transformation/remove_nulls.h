#pragma once

#include <string>

#include "transform_base.h"

namespace SrSecurity {
namespace Transformation {
class RemoveNulls : public TransformBase {
  DECLARE_TRANSFORM_NAME(removeNulls);

public:
  std::string evaluate(std::string_view data) const override {
    // Removes all NUL bytes from input.
    std::string result;
    result.reserve(data.length());
    for (size_t i = 0; i < data.length(); ++i) {
      if (data[i] != '\0') {
        result.push_back(data[i]);
      }
    }
    return result;
  }
};
} // namespace Transformation
} // namespace SrSecurity
