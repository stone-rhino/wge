#pragma once

#include <string_view>
#include <unordered_map>
#include <vector>

#include "../../config.h"

namespace SrSecurity {
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

  const std::vector<std::unordered_multimap<std::string_view, std::string_view>::iterator>&
  getNameValueLinked() const {
    return name_value_linked_;
  }

  const std::unordered_multimap<std::string_view, std::string_view>& getNameFileName() const {
    return name_filename_map_;
  }

  const std::vector<std::unordered_multimap<std::string_view, std::string_view>::iterator>&
  getNameFileNameLinked() const {
    return name_filename_linked_;
  }

  const std::unordered_multimap<std::string_view, std::string_view>& getHeaders() const {
    return headers_map_;
  }

  const std::vector<std::unordered_multimap<std::string_view, std::string_view>::iterator>&
  getHeadersLinked() const {
    return headers_linked_;
  }

  const MultipartStrictError& getError() const { return multipart_strict_error_; }

private:
  std::unordered_multimap<std::string_view, std::string_view> name_value_map_;
  std::vector<std::unordered_multimap<std::string_view, std::string_view>::iterator>
  name_value_linked_;
  std::unordered_multimap<std::string_view, std::string_view> name_filename_map_;
  std::vector<std::unordered_multimap<std::string_view, std::string_view>::iterator>
  name_filename_linked_;
  std::unordered_multimap<std::string_view, std::string_view> headers_map_;
  std::vector<std::unordered_multimap<std::string_view, std::string_view>::iterator>
  headers_linked_;
  MultipartStrictError multipart_strict_error_;
};
} // namespace Ragel
} // namespace Common
} // namespace SrSecurity