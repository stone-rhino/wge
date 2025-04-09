#pragma once

#include <string_view>

#include "transform_base.h"

#include "../common/empty_string.h"

namespace SrSecurity {
namespace Transformation {
class HexEncode : public TransformBase {
  DECLARE_TRANSFORM_NAME(hexEncode);

public:
  bool evaluate(std::string_view data, std::string& result) const override {
    result.clear();

    // To hex string
    result.resize(data.length() * 2);
    char* pr = result.data();
    for (size_t i = 0; i < data.length(); ++i) {
      pr[i * 2] = hex_table_[*(data.data() + i) >> 4];
      pr[i * 2 + 1] = hex_table_[*(data.data() + i) & 0x0f];
    }

    return true;
  }

private:
  static constexpr std::string_view hex_table_{"0123456789abcdef"};
};
} // namespace Transformation
} // namespace SrSecurity
