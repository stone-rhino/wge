#include "compress_whitespace.h"

#include <compress_whitespace.h>

namespace SrSecurity {
namespace Transformation {
bool CompressWhiteSpace::evaluate(std::string_view data, std::string& result) const {
  return compressWhitespace(data, result);
}
} // namespace Transformation
} // namespace SrSecurity
