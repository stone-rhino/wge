#pragma once
#include <functional>
#include <string_view>

namespace SrSecurity {

using UriExtractor = std::function<std::string_view()>;
using HeaderExtractor =
    std::function<void(const std::string_view& key, std::vector<std::string_view>& values)>;
using BodyExtractor = std::function<void(std::vector<std::string_view>& body_slices)>;

/**
 * Http message info extractor
 */
struct HttpExtractor {
  UriExtractor uri_extractor_;
  HeaderExtractor request_header_extractor_;
  HeaderExtractor response_header_extractor_;
  BodyExtractor reqeust_body_extractor_;
  BodyExtractor response_body_extractor_;
};
} // namespace SrSecurity