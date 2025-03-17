#pragma once

#include <string>

#include "transform_base.h"

namespace SrSecurity {
namespace Transformation {
class Utf8ToUnicode : public TransformBase {
  DECLARE_TRANSFORM_NAME(utf8ToUnicode);

public:
  std::string evaluate(std::string_view data) const override {
    // The utf-8 to unicode transformation.
    std::string result;
    result.reserve(data.length() * 2);
    for (size_t i = 0; i < data.length(); ++i) {
      if ((data[i] & 0x80) == 0) {
        result.push_back(data[i]);
      } else if ((data[i] & 0xe0) == 0xc0) {
        if (i + 1 < data.length()) {
          result.push_back(((data[i] & 0x1f) << 6) | (data[i + 1] & 0x3f));
          ++i;
        }
      } else if ((data[i] & 0xf0) == 0xe0) {
        if (i + 2 < data.length()) {
          result.push_back(((data[i] & 0x0f) << 12) | ((data[i + 1] & 0x3f) << 6) |
                           (data[i + 2] & 0x3f));
          i += 2;
        }
      } else if ((data[i] & 0xf8) == 0xf0) {
        if (i + 3 < data.length()) {
          result.push_back(((data[i] & 0x07) << 18) | ((data[i + 1] & 0x3f) << 12) |
                           ((data[i + 2] & 0x3f) << 6) | (data[i + 3] & 0x3f));
          i += 3;
        }
      }
    }
    return result;
  }
};
} // namespace Transformation
} // namespace SrSecurity
