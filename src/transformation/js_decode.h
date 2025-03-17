#pragma once

#include <string>

#include "transform_base.h"

namespace SrSecurity {
namespace Transformation {
class JsDecode : public TransformBase {
  DECLARE_TRANSFORM_NAME(jsDecode);

public:
  std::string evaluate(std::string_view data) const override {
    // Decodes JavaScript escape sequences. If a \uHHHH code is in the range of FF01-FF5E (the full
    // width ASCII codes), then the higher byte is used to detect and adjust the lower byte.
    // Otherwise, only the lower byte will be used and the higher byte zeroed (leading to possible
    // loss of information).
    std::string result;
    for (size_t i = 0; i < data.length(); ++i) {
      if (data[i] == '\\' && i + 1 < data.length() && data[i + 1] == 'u') {
        if (i + 5 < data.length()) {
          char c = 0;
          for (size_t j = 0; j < 4; ++j) {
            c <<= 4;
            if (data[i + 2 + j] >= '0' && data[i + 2 + j] <= '9') {
              c |= data[i + 2 + j] - '0';
            } else if (data[i + 2 + j] >= 'a' && data[i + 2 + j] <= 'f') {
              c |= data[i + 2 + j] - 'a' + 10;
            } else if (data[i + 2 + j] >= 'A' && data[i + 2 + j] <= 'F') {
              c |= data[i + 2 + j] - 'A' + 10;
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
          result.push_back(data[i]);
        }
      } else {
        result.push_back(data[i]);
      }
    }
    return result;
  }
};
} // namespace Transformation
} // namespace SrSecurity
