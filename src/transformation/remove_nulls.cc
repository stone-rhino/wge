#include "remove_nulls.h"

#include <remove_nulls.h>

namespace SrSecurity {
namespace Transformation {
bool RemoveNulls::evaluate(std::string_view data, std::string& result) const {
  return removeNulls(data, result);
}
} // namespace Transformation
} // namespace SrSecurity
