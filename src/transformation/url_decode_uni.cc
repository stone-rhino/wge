#include "url_decode_uni.h"

#include "hex_decode.h"

namespace SrSecurity {
namespace Transformation {
std::string UrlDecodeUni::evaluate(const void* data, size_t data_len) const {
  std::string result;
  result.reserve(data_len);
  HexDecode hex_decode;
  for (size_t i = 0; i < data_len; ++i) [[likely]] {
    const char& ch = static_cast<const char*>(data)[i];
    switch (ch) {
    case '%':
      if (i + 2 < data_len) [[likely]] {
        result += hex_decode.evaluate(static_cast<const char*>(data) + i + 1, 2);
        i += 2;
      }
      break;
    case '+':
      result += ' ';
      break;
    [[likely]] default:
      result += ch;
      break;
    }
  }

  return result;
}
} // namespace Transformation
} // namespace SrSecurity
