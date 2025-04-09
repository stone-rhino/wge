#include "hex_decode.h"

#include <hex_decode.h>

namespace SrSecurity {
namespace Transformation {
bool HexDecode::evaluate(std::string_view data, std::string& result) const {
  return hexDecode(data, result);
}
} // namespace Transformation
} // namespace SrSecurity
