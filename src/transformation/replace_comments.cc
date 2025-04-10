#include "replace_comments.h"

#include <replace_comments.h>

namespace SrSecurity {
namespace Transformation {
bool ReplaceComments::evaluate(std::string_view data, std::string& result) const {
  return replaceComments(data, result);
}
} // namespace Transformation
} // namespace SrSecurity
