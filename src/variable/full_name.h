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
