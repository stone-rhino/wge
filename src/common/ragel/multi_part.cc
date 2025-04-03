#include "multi_part.h"

#include <multi_part.h>

namespace SrSecurity {
namespace Common {
namespace Ragel {
void MultiPart::init(std::string_view content_type, std::string_view multi_part,
                     uint32_t max_file_count) {
  std::string_view boundary = ::parseContentType(content_type, multipart_strict_error_);
  if (boundary.empty()) {
    return;
  }
  ::parseMultiPart(multi_part, query_param_map_, query_param_linked_, multipart_strict_error_,
                   max_file_count);
}

} // namespace Ragel
} // namespace Common
} // namespace SrSecurity