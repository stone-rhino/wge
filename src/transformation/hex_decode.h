#pragma once

#include <string_view>

#include "transform_base.h"

namespace SrSecurity {
namespace Transformation {
class HexDecode : public TransformBase {
public:
  std::string evaluate(const void* data, size_t data_len) const override {
    std::string result;

    // Check the input
    if (data == nullptr || data_len == 0) [[unlikely]] {
      return result;
    }

    // To hex value
    result.resize(data_len / 2 + data_len % 2);
    const char* pch = reinterpret_cast<const char*>(data);
    char* pr = result.data();
    size_t len = 0;
    for (size_t i = 0; i < data_len; ++i) {
      auto pos = hex_table_.find(pch[i]);
      if (pos == std::string::npos) {
        result.clear();
        return result;
      }

      if (i % 2 == 0) {
        ++len;
        pr[len - 1] = static_cast<char>(pos << 4);
      } else {
        pr[len - 1] |= static_cast<char>(pos);
      }
    }

    return result;
  }

private:
  static constexpr std::string_view hex_table_{"0123456789abcdef"};
};
} // namespace Transformation
} // namespace SrSecurity
