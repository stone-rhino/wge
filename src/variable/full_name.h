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

namespace Wge {
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

  friend size_t hash_value(const FullName& full_name) {
    return std::hash<const char*>()(full_name.main_name_.data()) ^
           (std::hash<std::string_view>()(full_name.sub_name_) << 2);
  }
};
} // namespace Variable
} // namespace Wge

/**
 * Hash function for FullName.
 */
namespace std {
template <> struct hash<Wge::Variable::FullName> {
  size_t operator()(const Wge::Variable::FullName& full_name) const {
    return hash_value(full_name);
  }
};
} // namespace std
