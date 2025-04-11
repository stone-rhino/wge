#include "remove_comments_char.h"

#include <remove_comments_char.h>

namespace SrSecurity {
namespace Transformation {
bool RemoveCommentsChar::evaluate(std::string_view data, std::string& result) const {
  return removeCommentsChar(data, result);
}

} // namespace Transformation
} // namespace SrSecurity
