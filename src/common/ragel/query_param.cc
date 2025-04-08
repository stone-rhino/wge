#include "query_param.h"

#include <query_param.h>

namespace SrSecurity {
namespace Common {
namespace Ragel {
void QueryParam::init(std::string_view query_param_str) {
  query_param_map_.reserve(5);
  query_param_linked_.reserve(5);
  ::parseQueryParam(query_param_str, query_param_map_, query_param_linked_);
}

} // namespace Ragel
} // namespace Common
} // namespace SrSecurity