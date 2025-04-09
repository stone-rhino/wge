#include "html_entity_decode.h"

#include <html_entity_decode.h>

namespace SrSecurity {
namespace Transformation {
bool HtmlEntityDecode::evaluate(std::string_view data, std::string& result) const {
  return htmlEntityDecode(data, result);
}
} // namespace Transformation
} // namespace SrSecurity
