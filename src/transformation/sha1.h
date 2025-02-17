#pragma once

#include <string>

#include "transform_base.h"

namespace SrSecurity {
namespace Transformation {
class Sha1 : public TransformBase {
public:
  std::string evaluate(const void* data, size_t data_len) const override;
};
} // namespace Transformation
} // namespace SrSecurity
