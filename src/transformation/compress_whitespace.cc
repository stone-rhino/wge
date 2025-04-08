#include "compress_whitespace.h"
#include <compress_whitespace.h>

namespace SrSecurity {
namespace Transformation {
std::string CompressWhiteSpace::evaluate(std::string_view data) const {
  return compressWhitespace(data);
}
} // namespace Transformation
} // namespace SrSecurity
