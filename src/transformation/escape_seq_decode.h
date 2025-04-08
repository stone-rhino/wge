#pragma once

#include <string>

#include "transform_base.h"

namespace SrSecurity {
namespace Transformation {
class EscapeSeqDecode : public TransformBase {
  DECLARE_TRANSFORM_NAME(escapeSeqDecode);

public:
  std::string evaluate(std::string_view data) const override;
};
} // namespace Transformation
} // namespace SrSecurity
