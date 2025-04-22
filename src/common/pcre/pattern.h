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
#include <unordered_map>
#include <vector>

#include <stdint.h>

#include "scratch.h"

namespace SrSecurity {
namespace Common {
namespace Pcre {
class Pattern {
public:
  Pattern(const std::string& pattern, bool case_less, bool capture);
  Pattern(std::string_view pattern, bool case_less, bool capture);
  Pattern(const Pattern&) = delete;
  ~Pattern();

public:
  void* db() const { return db_; }

private:
  void compile(const std::string& pattern, bool case_less, bool capture);
  void compile(const std::string_view pattern, bool case_less, bool capture);

private:
  void* db_;
};

class PatternList {
public:
  void add(const std::string& pattern, bool case_less, bool capture, uint64_t id);
  const Pattern* get(uint64_t id) const;
  void clear() { pattern_map_.clear(); }

private:
  std::unordered_map<uint64_t, Pattern> pattern_map_;
};
} // namespace Pcre
} // namespace Common
} // namespace SrSecurity