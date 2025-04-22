/**
 * Copyright (c) 2024-2025 Stone Rhino and contributors.
 *
 * MIT License (http://opensource.org/licenses/MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
 * associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
 * NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
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