#include "url_decode_uni.h"

#include <boost/url.hpp>

namespace SrSecurity {
namespace Transformation {
bool UrlDecodeUni::evaluate(std::string_view data, std::string& result) const {
  boost::urls::pct_string_view pct_str(data);
  result = pct_str.decode();
  return true;
}
} // namespace Transformation
} // namespace SrSecurity
