#include "normalise_path.h"

#include <normalize_path.h>

namespace SrSecurity {
namespace Transformation {
bool NormalisePath::evaluate(std::string_view data, std::string& result) const {
  return normalizePath(data, result);
}

} // namespace Transformation
} // namespace SrSecurity
