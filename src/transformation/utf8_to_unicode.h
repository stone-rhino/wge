#pragma once

#include <string>

#include "transform_base.h"

namespace SrSecurity {
namespace Transformation {
class Utf8ToUnicode : public TransformBase {
  DECLARE_TRANSFORM_NAME(utf8ToUnicode);

public:
  bool evaluate(std::string_view data, std::string& result) const override;
};
} // namespace Transformation
} // namespace SrSecurity
