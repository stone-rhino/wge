#pragma once

#include <string_view>

#include "transform_base.h"

#include "../common/empty_string.h"

namespace SrSecurity {
namespace Transformation {
class HexEncode : public TransformBase {
  DECLARE_TRANSFORM_NAME(hexEncode);

public:
  std::string evaluate(const void* data, size_t data_len) const override {
    std::string result;

    // Check the input
    if (data == nullptr || data_len == 0) [[unlikely]] {
      return result;
    }

    // To hex string
    result.resize(data_len * 2);
    char* pr = reinterpret_cast<char*>(result.data());
    for (size_t i = 0; i < data_len; ++i) {
      pr[i * 2] = hex_table_[*(reinterpret_cast<const unsigned char*>(data) + i) >> 4];
      pr[i * 2 + 1] = hex_table_[*(reinterpret_cast<const unsigned char*>(data) + i) & 0x0f];
    }

    return result;
  }

private:
  static constexpr std::string_view hex_table_{"0123456789abcdef"};
};
} // namespace Transformation
} // namespace SrSecurity
