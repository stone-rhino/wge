#include "hex_decode.h"
#include <hex_decode.h>

namespace SrSecurity {
namespace Transformation {
std::string HexDecode::evaluate(std::string_view data) const {
  return hexDecode(data);
}
} // namespace Transformation
} // namespace SrSecurity
