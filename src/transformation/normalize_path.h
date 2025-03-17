#pragma once

#include <string>

#include "transform_base.h"

namespace SrSecurity {
namespace Transformation {
class NormalizePath : public TransformBase {
  DECLARE_TRANSFORM_NAME(normalizePath);

public:
  std::string evaluate(std::string_view data) const override {
    // Removes multiple slashes, directory self-references, and directory back-references (except
    // when at the beginning of the input) from input string.
    std::string result;
    result.reserve(data.length());
    size_t i = 0;
    while (i < data.length()) {
      if (data[i] == '/') {
        result.push_back(data[i]);
        ++i;
        while (i < data.length() && data[i] == '/') {
          ++i;
        }
      } else if (data[i] == '.' && i + 1 < data.length() && data[i + 1] == '/') {
        i += 2;
      } else if (data[i] == '.' && i + 2 < data.length() && data[i + 1] == '.' &&
                 data[i + 2] == '/') {
        if (result.size() > 1) {
          result.pop_back();
          while (result.size() > 1 && result.back() != '/') {
            result.pop_back();
          }
        }
        i += 3;
      } else {
        result.push_back(data[i]);
        ++i;
      }
    }
    return result;
  }
};
} // namespace Transformation
} // namespace SrSecurity
