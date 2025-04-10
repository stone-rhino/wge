#include "utf8_to_unicode.h"

#include <utf8_to_unicode.h>

namespace SrSecurity {
namespace Transformation {
bool Utf8ToUnicode::evaluate(std::string_view data, std::string& result) const {
  return utf8ToUnicode(data, result);
}
} // namespace Transformation
} // namespace SrSecurity
