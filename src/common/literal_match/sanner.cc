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
#include <algorithm>

#include "scanner.h"

#include "../assert.h"

namespace Wge {
namespace Common {
namespace LiteralMatch {
Scanner::Scanner(const std::string& pattern, bool case_less)
    : Scanner(std::string_view(pattern), case_less) {}

Scanner::Scanner(std::string_view pattern, bool case_less) : case_less_(case_less) {
  determineType(pattern);
}

bool Scanner::isLiteralPattern(std::string_view pattern) {
  static const std::string regex_chars = ".*+?{}[]|()\\";

  return pattern.find_first_of(regex_chars) == std::string_view::npos;
}

void Scanner::match(std::string_view subject,
                    std::vector<std::pair<size_t, size_t>>& result) const {
  std::string lower_subject;
  if (case_less_) {
    lower_subject.reserve(subject.size());
    std::transform(subject.begin(), subject.end(), std::back_inserter(lower_subject),
                   [](unsigned char c) { return std::tolower(c); });
    subject = lower_subject;
  }

  switch (type_) {
  case LiteralType::Empty: {
    if (subject.empty()) {
      result.emplace_back(0, 0);
    }
  } break;
  case LiteralType::NotEmpty: {
    if (!subject.empty()) {
      result.emplace_back(0, 0);
    }
  } break;
  case LiteralType::Exact: {
    if (subject == pattern_) {
      result.emplace_back(0, subject.size());
    }
  } break;
  case LiteralType::Prefix: {
    if (subject.starts_with(pattern_)) {
      result.emplace_back(0, pattern_.size());
    }
  } break;
  case LiteralType::Suffix: {
    if (subject.ends_with(pattern_)) {
      result.emplace_back(subject.size() - pattern_.size(), subject.size());
    }
  } break;
  case LiteralType::SubString: {
    auto pos = subject.find(pattern_);
    if (pos != std::string_view::npos) {
      result.emplace_back(pos, pos + pattern_.size());
    }
  } break;
  default:
    UNREACHABLE();
    break;
  }
}

void Scanner::determineType(std::string_view pattern) {
  bool has_anchor_start = pattern.starts_with("^");
  bool has_anchor_end = pattern.ends_with("$");
  bool has_case_insensitive = pattern.starts_with("(?i)");

  std::string_view actual_pattern = pattern;
  if (has_case_insensitive) {
    // Remove "(?i)"
    actual_pattern.remove_prefix(4);
    case_less_ = true;
  }
  if (has_anchor_start) {
    // Remove "^"
    actual_pattern.remove_prefix(1);
  }
  if (has_anchor_end) {
    // Remove "$"
    actual_pattern.remove_suffix(1);
  }

  if (case_less_) {
    // Convert to lower case for case-insensitive matching
    pattern_.clear();
    pattern_.reserve(actual_pattern.size());
    std::transform(actual_pattern.begin(), actual_pattern.end(), std::back_inserter(pattern_),
                   [](unsigned char c) { return std::tolower(c); });
  } else {
    pattern_ = actual_pattern;
  }

  if (pattern_.empty()) {
    type_ = LiteralType::Empty;
  } else if (pattern_ == ".+") {
    type_ = LiteralType::NotEmpty;
  } else if (has_anchor_start && has_anchor_end) {
    type_ = LiteralType::Exact;
  } else if (has_anchor_start) {
    type_ = LiteralType::Prefix;
  } else if (has_anchor_end) {
    type_ = LiteralType::Suffix;
  } else {
    type_ = LiteralType::SubString;
  }
}
} // namespace LiteralMatch
} // namespace Common
} // namespace Wge