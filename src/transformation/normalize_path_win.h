#pragma once

#include <string>

#include "transform_base.h"

namespace SrSecurity {
namespace Transformation {
class NormalizePathWin : public TransformBase {
  DECLARE_TRANSFORM_NAME(normalizePathWin);

public:
  std::string evaluate(const void* data, size_t data_len) const override {
    // Removes multiple slashes, directory self-references, and directory back-references (except
    // when at the beginning of the input) from input string.
    std::string result;
    std::string_view data_view(static_cast<const char*>(data), data_len);
    size_t i = 0;
    while (i < data_len) {
      if (data_view[i] == '\\') {
        result.push_back('/');
        ++i;
        while (i < data_len && data_view[i] == '\\') {
          ++i;
        }
      } else {
        result.push_back(data_view[i]);
        ++i;
      }
    }
    return result;
  }
};
} // namespace Transformation
} // namespace SrSecurity
