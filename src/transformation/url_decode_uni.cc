#include "url_decode_uni.h"

#include <boost/url.hpp>

namespace SrSecurity {
namespace Transformation {
std::string UrlDecodeUni::evaluate(std::string_view data) const {
  boost::urls::pct_string_view pct_str(data);
  return pct_str.decode();
}
} // namespace Transformation
} // namespace SrSecurity
