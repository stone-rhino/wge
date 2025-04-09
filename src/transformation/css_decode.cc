#include "css_decode.h"

#include <css_decode.h>

namespace SrSecurity {
namespace Transformation {
bool CssDecode::evaluate(std::string_view data, std::string& result) const {
  return cssDecode(data, result);
}
} // namespace Transformation
} // namespace SrSecurity
