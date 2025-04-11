#include "lowercase.h"

#include <lowercase.h>

namespace SrSecurity {
namespace Transformation {
bool LowerCase::evaluate(std::string_view data, std::string& result) const {
  return lowercase(data, result);
}

} // namespace Transformation
} // namespace SrSecurity
