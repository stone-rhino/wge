#include "normalize_path.h"

#include <filesystem>
#include <normalize_path.h>

namespace SrSecurity {
namespace Transformation {
bool NormalizePath::evaluate(std::string_view data, std::string& result) const {
  return normalizePath(data, result);
}
} // namespace Transformation
} // namespace SrSecurity
