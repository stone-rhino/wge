#include "base64_decode.h"

#include <base64_decode.h>

namespace SrSecurity {
namespace Transformation {
bool Base64Decode::evaluate(std::string_view data, std::string& result) const {
  return base64Decode(data, result);
}
} // namespace Transformation
} // namespace SrSecurity
