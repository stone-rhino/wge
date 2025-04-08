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
  name_value_map_.reserve(5);
  name_value_linked_.reserve(5);
  name_filename_map_.reserve(5);
  name_filename_linked_.reserve(5);
  ::parseMultiPart(multi_part, boundary, name_value_map_, name_value_linked_, name_filename_map_,
                   name_filename_linked_, headers_map_, headers_linked_, multipart_strict_error_,
                   max_file_count);
}

} // namespace Ragel
} // namespace Common
} // namespace SrSecurity