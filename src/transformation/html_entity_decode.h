#pragma once

#include <array>
#include <map>
#include <regex>
#include <string>

#include "transform_base.h"

#include "../common/assert.h"

namespace SrSecurity {
namespace Transformation {
class HtmlEntityDecode : public TransformBase {
  DECLARE_TRANSFORM_NAME(htmlEntityDecode);

public:
  std::string evaluate(std::string_view data) const override;
};
} // namespace Transformation
} // namespace SrSecurity
