#include "url_decode.h"

#include <url_decode.h>

namespace SrSecurity {
namespace Transformation {
bool UrlDecode::evaluate(std::string_view data, std::string& result) const {
  return urlDecode(data, result);
}
} // namespace Transformation
} // namespace SrSecurity
