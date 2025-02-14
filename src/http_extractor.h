#pragma once
#include <functional>
#include <string_view>

namespace SrSecurity {

using ConnectionExtractor =
    std::function<void(std::string_view& downstream_ip, short& downstream_port,
                       std::string_view& upstream_ip, short& upstream_port)>;

using UriExtractor = std::function<void(std::string_view& method, std::string_view& path,
                                        std::string_view& version)>;

using HeaderExtractor = std::function<std::vector<std::string_view>(const std::string& key)>;

using BodyExtractor = std::function<std::vector<std::string_view>()>;

/**
 * Http message info extractor
 */
struct HttpExtractor {
  ConnectionExtractor connection_extractor_;
  UriExtractor uri_extractor_;
  HeaderExtractor request_header_extractor_;
  HeaderExtractor response_header_extractor_;
  BodyExtractor reqeust_body_extractor_;
  BodyExtractor response_body_extractor_;
};
} // namespace SrSecurity