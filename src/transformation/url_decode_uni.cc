#include "url_decode_uni.h"

#include <boost/url.hpp>

namespace SrSecurity {
namespace Transformation {
std::string UrlDecodeUni::evaluate(const void* data, size_t data_len) const {
  boost::urls::pct_string_view pct_str(reinterpret_cast<const char*>(data), data_len);
  return pct_str.decode();
}
} // namespace Transformation
} // namespace SrSecurity
