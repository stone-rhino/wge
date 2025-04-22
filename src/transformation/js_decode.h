/**
 * Copyright (c) 2024-2025 Stone Rhino and contributors.
 *
 * MIT License (http://opensource.org/licenses/MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
 * associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
 * NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#pragma once

#include <string>

#include "transform_base.h"

namespace SrSecurity {
namespace Transformation {
class JsDecode : public TransformBase {
  DECLARE_TRANSFORM_NAME(jsDecode);

public:
  bool evaluate(std::string_view data, std::string& result) const override {
    // Decodes JavaScript escape sequences. If a \uHHHH code is in the range of FF01-FF5E (the full
    // width ASCII codes), then the higher byte is used to detect and adjust the lower byte.
    // Otherwise, only the lower byte will be used and the higher byte zeroed (leading to possible
    // loss of information).
    result.clear();
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
    return true;
  }
};
} // namespace Transformation
} // namespace SrSecurity
