#pragma once

#include <string>

#include "modsecurity/util.h"
#include "transform_base.h"

namespace SrSecurity {
namespace Transformation {
class CssDecode : public TransformBase {
  DECLARE_TRANSFORM_NAME(cssDecode);

public:
  std::string evaluate(std::string_view data) const override {
    return ModSecurity::cssDecode(data);
  }
};
} // namespace Transformation
} // namespace SrSecurity
