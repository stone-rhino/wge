#pragma once

#include <string>

#include "transform_base.h"

namespace SrSecurity {
namespace Transformation {
class Utf8ToUnicode : public TransformBase {
  DECLARE_TRANSFORM_NAME(utf8ToUnicode);

public:
  std::string evaluate(const void* data, size_t data_len) const override {
    // The utf-8 to unicode transformation.
    std::string result;
    std::string_view data_view(static_cast<const char*>(data), data_len);
    for (size_t i = 0; i < data_len; ++i) {
      if ((data_view[i] & 0x80) == 0) {
        result.push_back(data_view[i]);
      } else if ((data_view[i] & 0xE0) == 0xC0) {
        result.push_back(((data_view[i] & 0x1F) << 6) | (data_view[i + 1] & 0x3F));
        ++i;
      } else if ((data_view[i] & 0xF0) == 0xE0) {
        result.push_back(((data_view[i] & 0x0F) << 12) | ((data_view[i + 1] & 0x3F) << 6) |
                         (data_view[i + 2] & 0x3F));
        i += 2;
      } else if ((data_view[i] & 0xF8) == 0xF0) {
        result.push_back(((data_view[i] & 0x07) << 18) | ((data_view[i + 1] & 0x3F) << 12) |
                         ((data_view[i + 2] & 0x3F) << 6) | (data_view[i + 3] & 0x3F));
        i += 3;
      }
    }
    return result;
  }
};
} // namespace Transformation
} // namespace SrSecurity
