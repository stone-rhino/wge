#pragma once
#include <functional>
#include <string_view>

namespace SrSecurity {
/**
 * Header info extractor.
 * @param key the key of the header.
 * @return the value of the header.
 */
using HeaderExtractor = std::function<std::string_view(const std::string& key)>;

/**
 * Body info extractor.
 * @return vector of string_view, each string_view is a slice of the body.
 */
using BodyExtractor = std::function<const std::vector<std::string_view>&()>;

/**
 * Http message info extractor
 */
struct HttpExtractor {
  HeaderExtractor request_header_extractor_;
  HeaderExtractor response_header_extractor_;
  BodyExtractor reqeust_body_extractor_;
  BodyExtractor response_body_extractor_;
};
} // namespace SrSecurity