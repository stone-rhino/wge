#pragma once

#include <string>

#include "transform_base.h"
#include "normalize_path.h"

namespace SrSecurity {
namespace Transformation {
class NormalisePath : public TransformBase {
  DECLARE_TRANSFORM_NAME(normalisePath);

public:
  bool evaluate(std::string_view data, std::string& result) const override {
    return normalize_path_.evaluate(data, result);
  }

private:
  NormalizePath normalize_path_;
};
} // namespace Transformation
} // namespace SrSecurity
