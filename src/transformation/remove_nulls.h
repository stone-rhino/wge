#pragma once

#include <string>

#include "transform_base.h"

namespace SrSecurity {
namespace Transformation {
class RemoveNulls : public TransformBase {
  DECLARE_TRANSFORM_NAME(removeNulls);

public:
  std::string evaluate(const void* data, size_t data_len) const override {
    // Removes all NUL bytes from input.
    std::string result;
    std::string_view data_view(static_cast<const char*>(data), data_len);
    for (size_t i = 0; i < data_len; ++i) {
      if (data_view[i] != '\0') {
        result.push_back(data_view[i]);
      }
    }
    return result;
  }
};
} // namespace Transformation
} // namespace SrSecurity
