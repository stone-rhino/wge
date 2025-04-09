#pragma once

#include <string>

#include "transform_base.h"

namespace SrSecurity {
namespace Transformation {
class RemoveNulls : public TransformBase {
  DECLARE_TRANSFORM_NAME(removeNulls);

public:
  bool evaluate(std::string_view data, std::string& result) const override {
    // Removes all NUL bytes from input.
    result.clear();
    result.reserve(data.length());
    for (size_t i = 0; i < data.length(); ++i) {
      if (data[i] != '\0') {
        result.push_back(data[i]);
      }
    }
    return true;
  }
};
} // namespace Transformation
} // namespace SrSecurity
