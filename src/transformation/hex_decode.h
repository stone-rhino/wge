#pragma once

#include <string_view>

#include "transform_base.h"

namespace SrSecurity {
namespace Transformation {
class HexDecode : public TransformBase {
  DECLARE_TRANSFORM_NAME(hexDecode);

public:
  bool evaluate(std::string_view data, std::string& result) const override;
};
} // namespace Transformation
} // namespace SrSecurity
