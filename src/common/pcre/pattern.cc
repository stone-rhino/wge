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
#include "pattern.h"

#ifndef PCRE2_STATIC
#define PCRE2_STATIC
#endif

#ifndef PCRE2_CODE_UNIT_WIDTH
#define PCRE2_CODE_UNIT_WIDTH 8
#else
#error PCRE2_CODE_UNIT_WIDTH was defined!
#endif

#include <pcre2.h>

#include "../log.h"

namespace SrSecurity {
namespace Common {
namespace Pcre {
Pattern::Pattern(const std::string& pattern, bool case_less, bool capture) : db_(nullptr) {
  compile(pattern, case_less, capture);
}

Pattern::Pattern(std::string_view pattern, bool case_less, bool capture) : db_(nullptr) {
  compile(pattern, case_less, capture);
}

Pattern::~Pattern() {
  if (db_) {
    pcre2_code_free(reinterpret_cast<pcre2_code_8*>(db_));
    db_ = nullptr;
  }
}

void Pattern::compile(const std::string& pattern, bool case_less, bool capture) {
  compile(std::string_view(pattern), case_less, capture);
}

void Pattern::compile(const std::string_view pattern, bool case_less, bool capture) {
  int error_number;
  PCRE2_SIZE error_offset;
  uint32_t flag = PCRE2_DOTALL | PCRE2_MULTILINE;
  if (case_less) {
    flag |= PCRE2_CASELESS;
  }
  if (!capture) {
    flag |= PCRE2_NO_AUTO_CAPTURE;
  }
  db_ = pcre2_compile(reinterpret_cast<const unsigned char*>(pattern.data()), pattern.length(),
                      flag, &error_number, &error_offset, nullptr);
  if (db_ == nullptr) [[unlikely]] {
    char buffer[256];
    pcre2_get_error_message(error_number, reinterpret_cast<unsigned char*>(buffer), sizeof(buffer));
    SRSECURITY_LOG_ERROR("pcre compile error: {}", buffer);
    return;
  }

  pcre2_jit_compile(reinterpret_cast<pcre2_code_8*>(db_), PCRE2_JIT_COMPLETE);
}

void PatternList::add(const std::string& pattern, bool case_less, bool capture, uint64_t id) {
  if (pattern_map_.find(id) != pattern_map_.end()) [[unlikely]] {
    SRSECURITY_LOG_ERROR("add pattern failure! there has same id: {} {}", id, pattern);
    return;
  }

  pattern_map_.emplace(std::piecewise_construct, std::forward_as_tuple(id),
                       std::forward_as_tuple(pattern, case_less, capture));
}

const Pattern* PatternList::get(uint64_t id) const {
  const auto iter = pattern_map_.find(id);
  if (iter == pattern_map_.end()) [[unlikely]] {
    return nullptr;
  }

  return &iter->second;
}
} // namespace Pcre
} // namespace Common
} // namespace SrSecurity