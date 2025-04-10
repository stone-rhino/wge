#include "normalize_path_win.h"

#include <normalize_path_win.h>

namespace SrSecurity {
namespace Transformation {
bool NormalizePathWin::evaluate(std::string_view data, std::string& result) const {
  return normalizePathWin(data, result);
}
} // namespace Transformation
} // namespace SrSecurity
