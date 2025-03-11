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

Scanner::Scanner(const std::string& pattern, bool case_less)
    : pattern_(std::make_unique<Pattern>(pattern, case_less)) {}

Scanner::Scanner(const std::string_view pattern, bool case_less)
    : pattern_(std::make_unique<Pattern>(pattern, case_less)) {}

Scanner::Scanner(const PatternList* pattern_list) : pattern_list_(pattern_list) {}

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
  int rc =
      pcre2_match(reinterpret_cast<const pcre2_code_8*>(pattern->db()),
                  reinterpret_cast<const unsigned char*>(subject.data()), subject.length(), 0, 0,
                  reinterpret_cast<pcre2_match_data_8*>(per_thread_scratch_.hanlde()), nullptr);
  if (rc < 0) [[unlikely]] {
    switch (rc) {
    case PCRE2_ERROR_NOMATCH:
      SRSECURITY_LOG_TRACE("pcre no match: {}", subject);
      break;
    default:
      break;
    }
    return;
  }

  assert(rc == 1);
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
} // namespace Pcre
} // namespace Common
} // namespace SrSecurity