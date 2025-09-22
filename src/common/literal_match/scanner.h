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
#include <string_view>
#include <vector>

namespace Wge {
namespace Common {
namespace LiteralMatch {
class Scanner {
public:
  Scanner(const std::string& pattern, bool case_less);
  Scanner(std::string_view pattern, bool case_less);

public:
  static bool isLiteralPattern(std::string_view pattern);

public:
  void match(std::string_view subject, std::vector<std::pair<size_t, size_t>>& result) const;

private:
  enum class LiteralType {
    Empty,     // ^$
    NotEmpty,  // ^.+$
    Exact,     // ^pattern$
    Prefix,    // ^pattern
    Suffix,    // pattern$
    SubString, // pattern
  };

private:
  void determineType(std::string_view pattern);

private:
  LiteralType type_{LiteralType::Exact};
  bool case_less_{false};
  std::string pattern_;
};
} // namespace LiteralMatch
} // namespace Common
} // namespace Wge