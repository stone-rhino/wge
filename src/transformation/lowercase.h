#pragma once

#include <string>

#include "transform_base.h"

namespace SrSecurity {
namespace Transformation {
class LowerCase : public TransformBase {
  DECLARE_TRANSFORM_NAME(lowercase);

public:
  bool evaluate(std::string_view data, std::string& result) const override {
    result.clear();
    result.reserve(data.length());
    for (size_t i = 0; i < data.length(); ++i) {
      result.push_back(std::tolower(data[i]));
    }
    return true;
  }
};
} // namespace Transformation
} // namespace SrSecurity
