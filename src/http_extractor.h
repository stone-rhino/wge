#pragma once
#include <functional>
#include <string_view>

namespace SrSecurity {

/**
 * Connection info extractor.
 * @param downstream_ip this reference used to recv the downstream IP.
 * @param downstream_port this reference used to recv the downstream port.
 * @param upstream_ip this reference used to recv the upstream IP.
 * @param upstream_port this reference used to recv the upstream port.
 */
using ConnectionExtractor =
    std::function<void(std::string_view& downstream_ip, short& downstream_port,
                       std::string_view& upstream_ip, short& upstream_port)>;

/**
 * Uri info extractor.
 * @param method this reference used to recv the method.
 * @param path this reference used to recv the path.
 * @param protocol this reference used to recv the protocol.
 * @param version this reference used to recv the version.
 */
using UriExtractor = std::function<void(std::string_view& method, std::string_view& path,
                                        std::string_view& protocol, std::string_view& version)>;

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
  ConnectionExtractor connection_extractor_;
  UriExtractor uri_extractor_;
  HeaderExtractor request_header_extractor_;
  HeaderExtractor response_header_extractor_;
  BodyExtractor reqeust_body_extractor_;
  BodyExtractor response_body_extractor_;
};
} // namespace SrSecurity