#pragma once

#include <string_view>

#include "transform_base.h"

namespace SrSecurity {
namespace Transformation {
class HexDecode : public TransformBase {
  DECLARE_TRANSFORM_NAME(hexDecode);

public:
  std::string evaluate(std::string_view data) const override;
};
} // namespace Transformation
} // namespace SrSecurity
