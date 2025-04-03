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
  void init(std::string_view content_type, std::string_view multi_part, uint32_t max_file_count = 0);

public:
  const std::unordered_map<std::string_view, std::string_view>& get() const {
    return query_param_map_;
  }

  const std::vector<std::unordered_map<std::string_view, std::string_view>::iterator>&
  getLinked() const {
    return query_param_linked_;
  }

  const MultipartStrictError& getError() const { return multipart_strict_error_; }

private:
  std::unordered_map<std::string_view, std::string_view> query_param_map_;
  std::vector<std::unordered_map<std::string_view, std::string_view>::iterator> query_param_linked_;
  MultipartStrictError multipart_strict_error_;
};
} // namespace Ragel
} // namespace Common
} // namespace SrSecurity