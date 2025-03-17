#pragma once

#include <string>

#include "transform_base.h"

namespace SrSecurity {
namespace Transformation {
class CssDecode : public TransformBase {
  DECLARE_TRANSFORM_NAME(cssDecode);

public:
  std::string evaluate(const void* data, size_t data_len) const override {
    // Decodes characters encoded using the CSS 2.x escape rules syndata.html#characters. This
    // function uses only up to two bytes in the decoding process, meaning that it is useful to
    // uncover ASCII characters encoded using CSS encoding (that wouldn’t normally be encoded), or
    // to counter evasion, which is a combination of a backslash and non-hexadecimal characters
    // (e.g., ja\vascript is equivalent to javascript)
    std::string result;
    std::string_view data_view(static_cast<const char*>(data), data_len);
    for (size_t i = 0; i < data_len; ++i) {
      if (data_view[i] == '\\' && i + 1 < data_len) {
        if (data_view[i + 1] == '\n') {
          ++i;
        } else if (data_view[i + 1] == '\r' && i + 2 < data_len && data_view[i + 2] == '\n') {
          i += 2;
        } else {
          result.push_back(data_view[i + 1]);
          ++i;
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
