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
#pragma once

#include <array>
#include <optional>

#include <arpa/inet.h>

#include "operator_base.h"

namespace Wge {
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
  IpMatch(std::string&& literal_value, bool is_not, std::string_view curr_rule_file_path)
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

  IpMatch(const std::shared_ptr<Macro::MacroBase> macro, bool is_not,
          std::string_view curr_rule_file_path)
      : OperatorBase(macro, is_not) {
    // Not supported macro expansion
    UNREACHABLE();
  }

public:
  bool evaluate(Transaction& t, const Common::Variant& operand) const override {
    if (IS_STRING_VIEW_VARIANT(operand)) [[likely]] {
      // Copy the operand to a null-terminated string for inet_pton
      std::string ip;
      ip = std::get<std::string_view>(operand);

      // Match the IP address
      // FIXME(zhouyu 2025-03-10): To avoid call inet_pton every time, may be we can add a
      // std::array<uint32_t, 4> type that store the ip address into the Common::Variant and
      // initialize it when Transaction::processConnection is called. But this will decrease the
      // maintainability of the code. I don't think it is worth it.
      if (!mask_.has_value()) {
        return is_not_ ^ (literal_value_ == ip);
      } else {
        if (!ipv6_.has_value()) {
          uint32_t ip_value;
          ::inet_pton(AF_INET, ip.data(), &ip_value);
          return is_not_ ^
                 (applyMask4(ip_value, mask_.value()) == applyMask4(ipv4_, mask_.value()));
        } else {
          std::array<uint32_t, 4> ip_value;
          const uint32_t* tt = ip_value.data();
          ::inet_pton(AF_INET6, ip.data(), ip_value.data());
          return is_not_ ^
                 (applyMask6(ip_value, mask_.value()) == applyMask6(ipv6_.value(), mask_.value()));
        }
      }
    } else {
      return false;
    }
  }

private:
  uint32_t applyMask4(uint32_t ip, uint32_t mask) const {
    return ip & htonl(0xFFFFFFFF << (32 - mask));
  }

  std::array<uint32_t, 4> applyMask6(const std::array<uint32_t, 4>& ip, uint32_t mask) const {
    std::array<uint32_t, 4> masked_ip = ip;
    uint32_t full_blocks = (128 - mask) / 32;
    uint32_t remaining_bits = (128 - mask) % 32;

    for (size_t i = 0; i < 4; ++i) {
      if (i < full_blocks) {
        // Full mask
        masked_ip[i] &= 0xFFFFFFFF;
      } else if (i == full_blocks) {
        // Partial mask
        masked_ip[i] &= htonl(0xFFFFFFFF << (32 - remaining_bits));
      } else {
        // No mask
        masked_ip[i] = 0;
      }
    }

    return masked_ip;
  };

private:
  uint32_t ipv4_;
  std::optional<std::array<uint32_t, 4>> ipv6_;
  std::optional<uint32_t> mask_;
};
} // namespace Operator
} // namespace Wge