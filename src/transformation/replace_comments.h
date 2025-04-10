#pragma once

#include <string>

#include "transform_base.h"

namespace SrSecurity {
namespace Transformation {
class ReplaceComments : public TransformBase {
  DECLARE_TRANSFORM_NAME(replaceComments);

public:
  bool evaluate(std::string_view data, std::string& result) const override;
};
} // namespace Transformation
} // namespace SrSecurity
