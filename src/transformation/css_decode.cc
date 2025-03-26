#include "css_decode.h"
#include <css_decode.h>

namespace SrSecurity {
namespace Transformation {
std::string CssDecode::evaluate(std::string_view data) const {
  return cssDecode(data);
}
} // namespace Transformation
} // namespace SrSecurity
