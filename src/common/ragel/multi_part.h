/**
 * Copyright (c) 2024-2026 Stone Rhino and contributors.
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
#pragma once

#include <string_view>
#include <unordered_map>
#include <vector>

#include "../../config.h"

namespace Wge {
namespace Common {
namespace Ragel {
/**
 * The class for parsing multipart/form-data content.
 */
class MultiPart {
public:
  void init(std::string_view content_type, std::string_view multi_part,
            uint32_t max_file_count = 0);

public:
  const std::unordered_multimap<std::string_view, std::string_view>& getNameValue() const {
    return name_value_map_;
  }

  const std::vector<std::pair<std::string_view, std::string_view>>& getNameValueLinked() const {
    return name_value_linked_;
  }

  const std::unordered_multimap<std::string_view, std::string_view>& getNameFileName() const {
    return name_filename_map_;
  }

  const std::vector<std::pair<std::string_view, std::string_view>>& getNameFileNameLinked() const {
    return name_filename_linked_;
  }

  const std::unordered_multimap<std::string_view, std::string_view>& getHeaders() const {
    return headers_map_;
  }

  const std::vector<std::pair<std::string_view, std::string_view>>& getHeadersLinked() const {
    return headers_linked_;
  }

  const std::vector<int64_t>& getFilesSizes() const { return files_sizes_; }

  int64_t getFileCombinedSize() const { return file_combined_size_; }

  int64_t getNoFilesSize() const { return no_files_size_; }

  const MultipartStrictError& getError() const { return multipart_strict_error_; }

private:
  std::unordered_multimap<std::string_view, std::string_view> name_value_map_;
  std::vector<std::pair<std::string_view, std::string_view>> name_value_linked_;
  std::unordered_multimap<std::string_view, std::string_view> name_filename_map_;
  std::vector<std::pair<std::string_view, std::string_view>> name_filename_linked_;
  std::unordered_multimap<std::string_view, std::string_view> headers_map_;
  std::vector<std::pair<std::string_view, std::string_view>> headers_linked_;
  std::vector<int64_t> files_sizes_;
  int64_t file_combined_size_{0};
  int64_t no_files_size_{0};
  MultipartStrictError multipart_strict_error_;
};
} // namespace Ragel
} // namespace Common
} // namespace Wge