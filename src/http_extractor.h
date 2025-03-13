#pragma once
#include <functional>
#include <string_view>

namespace SrSecurity {

/**
 * Header find function.
 * @param key the header key.
 * @return the header value. if the header does not exist, return empty string_view.
 */
using HeaderFind = std::function<std::string_view(const std::string& key)>;

/**
 * Header traversal callback.
 * @param key the header key.
 * @param value the header value.
 * @return true if continue traversal, false if stop traversal.
 */
using HeaderTraversalCallback = std::function<bool(std::string_view key, std::string_view value)>;

/**
 * Header traversal function.
 * @param callback the header traversal callback.
 */
using HeaderTraversal = std::function<void(HeaderTraversalCallback call)>;

/**
 * Body info extractor.
 * @return vector of string_view, each string_view is a slice of the body.
 */
using BodyExtractor = std::function<const std::vector<std::string_view>&()>;

/**
 * Http message info extractor
 */
struct HttpExtractor {
  HeaderFind request_header_find_;
  HeaderTraversal request_header_traversal_;
  HeaderFind response_header_find_;
  HeaderTraversal response_header_traversal_;
  BodyExtractor reqeust_body_extractor_;
  BodyExtractor response_body_extractor_;
  size_t request_header_count_;
  size_t response_header_count_;
};
} // namespace SrSecurity