#pragma once

#include <string>

#include <string.h>

namespace SrSecurity {
namespace Variable {
struct FullName {
  std::string_view main_name_;
  std::string_view sub_name_;

  std::string tostring() const {
    std::string full_name;
    full_name = main_name_;
    if (!sub_name_.empty()) {
      full_name += ":";
      full_name.append(sub_name_.data(), sub_name_.size());
    }
    return full_name;
  }

  bool operator==(const FullName& full_name) const {
    if (main_name_.data() == full_name.main_name_.data()) {
      return sub_name_ == full_name.sub_name_;
    }

    return false;
  }
};
} // namespace Variable
} // namespace SrSecurity

/**
 * Hash function for FullName.
 */
namespace std {
template <> struct hash<SrSecurity::Variable::FullName> {
  size_t operator()(const SrSecurity::Variable::FullName& s) const {
    size_t h1 = std::hash<const char*>()(s.main_name_.data());
    size_t h2 = std::hash<std::string_view>()(s.sub_name_);
    return h1 ^ (h2 << 1);
  }
};
} // namespace std
