#include "html_entity_decode.h"

#include <html_entity_decode.h>

namespace SrSecurity {
namespace Transformation {
std::string HtmlEntityDecode::evaluate(std::string_view data) const {
  return htmlEntityDecode(data);
}
} // namespace Transformation
} // namespace SrSecurity
