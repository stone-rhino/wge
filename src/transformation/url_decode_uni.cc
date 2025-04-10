#include "url_decode_uni.h"

#include <url_decode_uni.h>

namespace SrSecurity {
namespace Transformation {
bool UrlDecodeUni::evaluate(std::string_view data, std::string& result) const {
  return urlDecodeUni(data, result);
}
} // namespace Transformation
} // namespace SrSecurity
