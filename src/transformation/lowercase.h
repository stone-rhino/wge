#pragma once

#include <string>

#include "transform_base.h"

namespace SrSecurity {
namespace Transformation {
class LowerCase : public TransformBase {
  DECLARE_TRANSFORM_NAME(lowercase);

public:
  std::string evaluate(const void* data, size_t data_len) const override {
    std::string result;
    result.reserve(data_len);
    for (size_t i = 0; i < data_len; ++i) {
      result.push_back(std::tolower(static_cast<const char*>(data)[i]));
    }
    return result;
  }
};
} // namespace Transformation
} // namespace SrSecurity
