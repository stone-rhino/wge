#pragma once

#include <string>

#include "transform_base.h"

namespace SrSecurity {
namespace Transformation {
class JsDecode : public TransformBase {
  DECLARE_TRANSFORM_NAME(jsDecode);

public:
  std::string evaluate(const void* data, size_t data_len) const override {
    // Decodes JavaScript escape sequences. If a \uHHHH code is in the range of FF01-FF5E (the full
    // width ASCII codes), then the higher byte is used to detect and adjust the lower byte.
    // Otherwise, only the lower byte will be used and the higher byte zeroed (leading to possible
    // loss of information).
    std::string result;
    std::string_view data_view(static_cast<const char*>(data), data_len);
    for (size_t i = 0; i < data_len; ++i) {
      if (data_view[i] == '\\' && i + 1 < data_len && data_view[i + 1] == 'u') {
        if (i + 5 < data_len) {
          char c = 0;
          for (size_t j = 0; j < 4; ++j) {
            c <<= 4;
            if (data_view[i + 2 + j] >= '0' && data_view[i + 2 + j] <= '9') {
              c |= data_view[i + 2 + j] - '0';
            } else if (data_view[i + 2 + j] >= 'a' && data_view[i + 2 + j] <= 'f') {
              c |= data_view[i + 2 + j] - 'a' + 10;
            } else if (data_view[i + 2 + j] >= 'A' && data_view[i + 2 + j] <= 'F') {
              c |= data_view[i + 2 + j] - 'A' + 10;
            } else {
              break;
            }
          }
          if (c >= 0xFF01 && c <= 0xFF5E) {
            c -= 0xFEE0;
          }
          result.push_back(c);
          i += 5;
        } else {
          result.push_back(data_view[i]);
        }
      } else {
        result.push_back(data_view[i]);
      }
    }
    return result;
  }
};
} // namespace Transformation
} // namespace SrSecurity
