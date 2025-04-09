#pragma once

#include <string>

#include "transform_base.h"

namespace SrSecurity {
namespace Transformation {
class UrlDecodeUni : public TransformBase {
  DECLARE_TRANSFORM_NAME(urlDecodeUni);

public:
  bool evaluate(std::string_view data, std::string& result) const override;
};
} // namespace Transformation
} // namespace SrSecurity
