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
#include "scanner.h"

#ifndef PCRE2_STATIC
#define PCRE2_STATIC
#endif

#ifndef PCRE2_CODE_UNIT_WIDTH
#define PCRE2_CODE_UNIT_WIDTH 8
#else
#error PCRE2_CODE_UNIT_WIDTH was defined!
#endif

#include <assert.h>
#include <pcre2.h>

#include "../log.h"

namespace SrSecurity {
namespace Common {
namespace Pcre {
thread_local Scratch Scanner::per_thread_scratch_(99);

Scanner::Scanner(const std::string& pattern, bool case_less, bool captrue)
    : pattern_(std::make_unique<Pattern>(pattern, case_less, captrue)) {}

Scanner::Scanner(const std::string_view pattern, bool case_less, bool captrue)
    : pattern_(std::make_unique<Pattern>(pattern, case_less, captrue)) {}

Scanner::Scanner(const PatternList* pattern_list) : pattern_list_(pattern_list) {}

Scanner::~Scanner() {
  if (match_context_) {
    pcre2_match_context_free(static_cast<pcre2_match_context*>(match_context_));
  }
}

const Pattern* Scanner::getPattern(uint64_t id) {
  assert(pattern_list_);
  if (!pattern_list_) [[unlikely]] {
    return nullptr;
  }

  return pattern_list_->get(id);
}

void Scanner::match(std::string_view subject,
                    std::vector<std::pair<size_t, size_t>>& result) const {
  assert(pattern_);
  if (!pattern_) [[unlikely]] {
    return;
  }

  match(pattern_.get(), subject, result);
}

void Scanner::match(uint64_t id, std::string_view subject,
                    std::vector<std::pair<size_t, size_t>>& result) const {
  assert(pattern_list_);
  if (!pattern_list_) [[unlikely]] {
    return;
  }

  auto pattern = pattern_list_->get(id);
  if (pattern) [[likely]] {
    match(pattern, subject, result);
  }
}

void Scanner::match(const Pattern* pattern, std::string_view subject,
                    std::vector<std::pair<size_t, size_t>>& result) const {
  assert(pattern);

  int rc = pcre2_jit_match(static_cast<const pcre2_code_8*>(pattern->db()),
                           reinterpret_cast<const unsigned char*>(subject.data()), subject.length(),
                           0, 0, static_cast<pcre2_match_data_8*>(per_thread_scratch_.hanlde()),
                           static_cast<pcre2_match_context*>(match_context_));
  if (rc < 0) [[unlikely]] {
    switch (rc) {
    case PCRE2_ERROR_NOMATCH:
      SRSECURITY_LOG_TRACE("pcre no match: {}", subject);
      break;
    case PCRE2_ERROR_MATCHLIMIT:
      SRSECURITY_LOG_TRACE("pcre match limit", subject);
      break;
    default:
      break;
    }
    return;
  }

  if (rc == 0) [[unlikely]] {
    SRSECURITY_LOG_ERROR("ovector was not big enough for captured substring", subject);
    return;
  }

  auto ovector = pcre2_get_ovector_pointer(
      reinterpret_cast<pcre2_match_data_8*>(per_thread_scratch_.hanlde()));
  for (size_t i = 0; i < rc; i++) {
    result.emplace_back(std::make_pair(ovector[i * 2], ovector[i * 2 + 1]));
  }
}

bool Scanner::match(const Pattern* pattern, std::string_view subject) const {
  assert(pattern);

  int rc = pcre2_jit_match(static_cast<const pcre2_code_8*>(pattern->db()),
                           reinterpret_cast<const unsigned char*>(subject.data()), subject.length(),
                           0, 0, static_cast<pcre2_match_data_8*>(per_thread_scratch_.hanlde()),
                           static_cast<pcre2_match_context*>(match_context_));
  if (rc < 0) [[unlikely]] {
    switch (rc) {
    case PCRE2_ERROR_NOMATCH:
      SRSECURITY_LOG_TRACE("pcre no match: {}", subject);
      break;
    case PCRE2_ERROR_MATCHLIMIT:
      SRSECURITY_LOG_TRACE("pcre match limit", subject);
      break;
    default:
      break;
    }
    return false;
  }

  if (rc == 0) [[unlikely]] {
    SRSECURITY_LOG_ERROR("ovector was not big enough for captured substring", subject);
    return false;
  }

  return rc > 0;
}

bool Scanner::match(std::string_view subject) const { return match(pattern_.get(), subject); }

void Scanner::matchGlobal(std::string_view subject,
                          std::vector<std::pair<size_t, size_t>>& result) const {
  assert(pattern_);
  if (!pattern_) [[unlikely]] {
    return;
  }

  return matchGlobal(pattern_.get(), subject, result);
}

void Scanner::matchGlobal(uint64_t id, std::string_view subject,
                          std::vector<std::pair<size_t, size_t>>& result) const {
  assert(pattern_list_);
  if (!pattern_list_) [[unlikely]] {
    return;
  }

  auto pattern = pattern_list_->get(id);
  if (pattern) [[likely]] {
    return matchGlobal(pattern, subject, result);
  }
}

void Scanner::matchGlobal(const Pattern* pattern, std::string_view subject,
                          std::vector<std::pair<size_t, size_t>>& result) const {
  assert(pattern);
  int rc = 0;
  size_t start_offset = 0;
  do {
    rc = pcre2_match(reinterpret_cast<const pcre2_code_8*>(pattern->db()),
                     reinterpret_cast<const unsigned char*>(subject.data()), subject.length(),
                     start_offset, 0,
                     reinterpret_cast<pcre2_match_data_8*>(per_thread_scratch_.hanlde()), nullptr);
    if (rc == 1) {
      auto ovector = pcre2_get_ovector_pointer(
          reinterpret_cast<pcre2_match_data_8*>(per_thread_scratch_.hanlde()));
      result.emplace_back(std::make_pair(ovector[0], ovector[1]));
      start_offset = ovector[1] + 1;
    }
  } while (rc > 0);
}

void Scanner::setMatchLimit(size_t match_limit) {
  if (!match_context_) {
    match_context_ = pcre2_match_context_create(nullptr);
    pcre2_set_match_limit(static_cast<pcre2_match_context*>(match_context_), match_limit);
  }
}
} // namespace Pcre
} // namespace Common
} // namespace SrSecurity