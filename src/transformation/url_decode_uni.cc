#include "url_decode_uni.h"

#include "hex_decode.h"

namespace SrSecurity {
namespace Transformation {
std::string UrlDecodeUni::evaluate(const void* data, size_t data_len) const {
  std::string result;
  HexDecode hex_decode;
  for (size_t i = 0; i < data_len; ++i) {
    const char& ch = static_cast<const char*>(data)[i];
    if (ch == '%') {
      if (i + 2 < data_len) {
        result += hex_decode.evaluate(static_cast<const char*>(data) + i + 1, 2);
        i += 2;
      }
    } else if (ch == '+') {
      result += ' ';
    } else {
      result += ch;
    }
  }

  return result;
}
} // namespace Transformation
} // namespace SrSecurity
