#include "pcre.h"

#ifndef PCRE2_STATIC
#define PCRE2_STATIC
#endif

#ifndef PCRE2_CODE_UNIT_WIDTH
#define PCRE2_CODE_UNIT_WIDTH 8
#else
#error PCRE2_CODE_UNIT_WIDTH was defined!
#endif

#include <pcre2.h>

#include "log.h"

namespace SrSecurity {
namespace Common {
Pcre::Pcre(const std::string& pattern, bool case_less) : db_(nullptr) {
  compile(pattern, case_less);
}

Pcre::~Pcre() {
  if (db_) {
    pcre2_code_free(reinterpret_cast<pcre2_code_8*>(db_));
    db_ = nullptr;
  }
}

std::vector<std::pair<size_t, size_t>> Pcre::match(std::string_view subject,
                                                   Scratch& scratch) const {
  std::vector<std::pair<size_t, size_t>> result;
  assert(scratch.scratch_);
  if (!scratch.scratch_) [[unlikely]] {
    return result;
  }

  int rc = pcre2_match(reinterpret_cast<const pcre2_code_8*>(db_),
                       reinterpret_cast<const unsigned char*>(subject.data()), subject.length(), 0,
                       0, reinterpret_cast<pcre2_match_data_8*>(scratch.scratch_), nullptr);
  if (rc < 0) [[unlikely]] {
    switch (rc) {
    case PCRE2_ERROR_NOMATCH:
      SRSECURITY_LOG_TRACE("pcre no match: {}", subject);
      break;
    default:
      break;
    }
    return result;
  }

  if (rc == 0) {
    SRSECURITY_LOG_ERROR("ovector was not big enough for captured substring", subject);
    return result;
  }

  auto ovector = pcre2_get_ovector_pointer(reinterpret_cast<pcre2_match_data_8*>(scratch.scratch_));
  for (size_t i = 0; i < rc; i++) {
    result.emplace_back(std::make_pair(ovector[i * 2], ovector[i * 2 + 1]));
  }

  return result;
}

std::vector<std::pair<size_t, size_t>> Pcre::matchGlobal(std::string_view subject,
                                                         Scratch& scratch) const {
  std::vector<std::pair<size_t, size_t>> result;
  assert(scratch.scratch_);
  if (!scratch.scratch_) [[unlikely]] {
    return result;
  }

  int rc = 0;
  size_t start_offset = 0;
  do {
    rc = pcre2_match(reinterpret_cast<const pcre2_code_8*>(db_),
                     reinterpret_cast<const unsigned char*>(subject.data()), subject.length(),
                     start_offset, 0, reinterpret_cast<pcre2_match_data_8*>(scratch.scratch_),
                     nullptr);
    if (rc == 1) {
      auto ovector =
          pcre2_get_ovector_pointer(reinterpret_cast<pcre2_match_data_8*>(scratch.scratch_));
      result.emplace_back(std::make_pair(ovector[0], ovector[1]));
      start_offset = ovector[1] + 1;
    }
  } while (rc > 0);

  return result;
}

void Pcre::compile(const std::string& pattern, bool case_less) {
  int error_number;
  PCRE2_SIZE error_offset;
  db_ = pcre2_compile(reinterpret_cast<const unsigned char*>(pattern.c_str()), pattern.length(),
                      case_less ? PCRE2_CASELESS : 0, &error_number, &error_offset, nullptr);
  if (db_ == nullptr) [[unlikely]] {
    char buffer[256];
    pcre2_get_error_message(error_number, reinterpret_cast<unsigned char*>(buffer), sizeof(buffer));
    SRSECURITY_LOG_WARN("pcre compile error: {}", buffer);
    return;
  }
}

Pcre::Scratch::Scratch(int result_count) {
  scratch_ = pcre2_match_data_create(result_count, nullptr);
}

Pcre::Scratch::~Scratch() {
  if (scratch_) {
    pcre2_match_data_free(reinterpret_cast<pcre2_match_data*>(scratch_));
    scratch_ = nullptr;
  }
}

} // namespace Common
} // namespace SrSecurity