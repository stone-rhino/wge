#pragma once

#include <string>

#include "transform_base.h"

namespace SrSecurity {
namespace Transformation {
class RemoveCommentChar : public TransformBase {
public:
  void evaluate(const void* data, size_t data_len, void* buffer, size_t buffer_len) override {}
};
} // namespace Transformation
} // namespace SrSecurity
