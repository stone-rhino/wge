#include "remove_whitespace.h"

#include <remove_whitespace.h>

namespace SrSecurity {
namespace Transformation {
bool RemoveWhitespace::evaluate(std::string_view data, std::string& result) const {
  return removeWhitespace(data, result);
}
} // namespace Transformation
} // namespace SrSecurity
