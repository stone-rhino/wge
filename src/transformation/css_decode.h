#pragma once

#include <string>

#include "transform_base.h"

namespace SrSecurity {
namespace Transformation {
class CssDecode : public TransformBase {
  DECLARE_TRANSFORM_NAME(cssDecode);

public:
  std::string evaluate(std::string_view data) const override {
    // Decodes characters encoded using the CSS 2.x escape rules syndata.html#characters. This
    // function uses only up to two bytes in the decoding process, meaning that it is useful to
    // uncover ASCII characters encoded using CSS encoding (that wouldn’t normally be encoded), or
    // to counter evasion, which is a combination of a backslash and non-hexadecimal characters
    // (e.g., ja\vascript is equivalent to javascript)
    std::string result;
    for (size_t i = 0; i < data.length(); ++i) {
      if (data[i] == '\\' && i + 1 < data.length()) {
        if (data[i + 1] == '\n') {
          ++i;
        } else if (data[i + 1] == '\r' && i + 2 < data.length() && data[i + 2] == '\n') {
          i += 2;
        } else {
          result.push_back(data[i + 1]);
          ++i;
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
