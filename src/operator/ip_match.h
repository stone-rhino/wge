#pragma once

#include <array>
#include <optional>

#include <arpa/inet.h>

#include "operator_base.h"

namespace SrSecurity {
namespace Operator {
/**
 * Performs a fast ipv4 or ipv6 match of REMOTE_ADDR variable data. Can handle the following
 * formats:
 * Full IPv4 Address - 192.168.1.100
 * Network Block/CIDR Address - 192.168.1.0/24
 * Full IPv6 Address - 2001:db8:85a3:8d3:1319:8a2e:370:7348
 * Network Block/CIDR Address - 2001:db8:85a3:8d3:1319:8a2e:370:0/24
 */
class IpMatch : public OperatorBase {
  DECLARE_OPERATOR_NAME(ipMatch);

public:
  IpMatch(std::string&& literal_value, bool is_not)
      : OperatorBase(std::move(literal_value), is_not) {
    auto pos = literal_value_.find('/');
    if (pos == std::string::npos) {
      if (literal_value_.find(':') == std::string::npos) {
        ::inet_pton(AF_INET, literal_value_.c_str(), &ipv4_);
      } else {
        ipv6_ = std::make_optional<std::array<uint32_t, 4>>();
        ::inet_pton(AF_INET6, literal_value_.c_str(), &ipv6_);
      }
    } else {
      if (literal_value_.find(':') == std::string::npos) {
        ::inet_pton(AF_INET, literal_value_.substr(0, pos).c_str(), &ipv4_);
        mask_ = ::atoi(literal_value_.substr(pos + 1).c_str());
        mask_ = mask_ > 32 ? 32 : mask_;
      } else {
        ipv6_ = std::make_optional<std::array<uint32_t, 4>>();
        ::inet_pton(AF_INET6, literal_value_.substr(0, pos).c_str(), &ipv6_);
        mask_ = ::atoi(literal_value_.substr(pos + 1).c_str());
        mask_ = mask_ > 128 ? 128 : mask_;
      }
    }
  }

  IpMatch(const std::shared_ptr<Macro::MacroBase> macro, bool is_not)
      : OperatorBase(macro, is_not) {}

public:
  bool evaluate(Transaction& t, const Common::Variant& operand) const override {
    if (IS_STRING_VIEW_VARIANT(operand)) [[likely]] {
      return match(std::string(std::get<std::string_view>(operand)));
    } else if (IS_STRING_VARIANT(operand)) {
      return match(std::get<std::string>(operand));
    } else {
      return false;
    }
  }

private:
  bool match(const std::string& ip) const {
    if (!mask_.has_value()) {
      return literal_value_ == ip;
    } else {
      if (!ipv6_.has_value()) {
        uint32_t ip_value;
        ::inet_pton(AF_INET, ip.data(), &ip_value);
        return (ipv4_ >> (32 - mask_.value())) == (ip_value >> (32 - mask_.value()));
      } else {
        std::array<uint32_t, 4> ip_value;
        ::inet_pton(AF_INET6, ip.data(), ip_value.data());
        return (ipv6_.value()[0] >> (128 - mask_.value())) ==
                   (ip_value[0] >> (128 - mask_.value())) &&
               (ipv6_.value()[1] >> (128 - mask_.value())) ==
                   (ip_value[1] >> (128 - mask_.value())) &&
               (ipv6_.value()[2] >> (128 - mask_.value())) ==
                   (ip_value[2] >> (128 - mask_.value())) &&
               (ipv6_.value()[3] >> (128 - mask_.value())) ==
                   (ip_value[3] >> (128 - mask_.value()));
      }
    }
  }

private:
  uint32_t ipv4_;
  std::optional<std::array<uint32_t, 4>> ipv6_;
  std::optional<uint32_t> mask_;
};
} // namespace Operator
} // namespace SrSecurity