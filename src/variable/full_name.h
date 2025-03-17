#pragma once

#include <string>

#include <string.h>

namespace SrSecurity {
namespace Variable {
struct FullName {
  const char* main_name_;
  const std::string& sub_name_;

  std::string tostring() const {
    std::string full_name = main_name_;
    if (!sub_name_.empty()) {
      full_name += ":" + sub_name_;
    }
    return full_name;
  }

  bool operator>(const FullName& full_name) const {
    int result = ::strcmp(main_name_, full_name.main_name_);
    if (result == 0) {
      return sub_name_ > full_name.sub_name_;
    }

    return result > 0;
  }

  bool operator<(const FullName& full_name) const {
    int result = ::strcmp(main_name_, full_name.main_name_);
    if (result == 0) {
      return sub_name_ < full_name.sub_name_;
    }

    return result < 0;
  }

  bool operator==(const FullName& full_name) const {
    int result = ::strcmp(main_name_, full_name.main_name_);
    if (result == 0) {
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
    size_t h1 = std::hash<const char*>()(s.main_name_);
    size_t h2 = std::hash<std::string>()(s.sub_name_);
    return h1 ^ (h2 << 1);
  }
};
} // namespace std
