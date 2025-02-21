#pragma once

#include <string>

#include "transform_base.h"

namespace SrSecurity {
namespace Transformation {
class UrlDecodeUni : public TransformBase {
  DECLARE_TRANSFORM_NAME(urlDecodeUni);

public:
  std::string evaluate(const void* data, size_t data_len) const override;
};
} // namespace Transformation
} // namespace SrSecurity
