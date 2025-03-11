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
Pattern::Pattern(const std::string& pattern, bool case_less) : db_(nullptr) {
  compile(pattern, case_less);
}

Pattern::Pattern(std::string_view pattern, bool case_less) : db_(nullptr) {
  compile(pattern, case_less);
}

Pattern::~Pattern() {
  if (db_) {
    pcre2_code_free(reinterpret_cast<pcre2_code_8*>(db_));
    db_ = nullptr;
  }
}

void Pattern::compile(const std::string& pattern, bool case_less) {
  compile(std::string_view(pattern), case_less);
}

void Pattern::compile(const std::string_view pattern, bool case_less) {
  int error_number;
  PCRE2_SIZE error_offset;
  db_ = pcre2_compile(reinterpret_cast<const unsigned char*>(pattern.data()), pattern.length(),
                      case_less ? PCRE2_CASELESS : 0, &error_number, &error_offset, nullptr);
  if (db_ == nullptr) [[unlikely]] {
    char buffer[256];
    pcre2_get_error_message(error_number, reinterpret_cast<unsigned char*>(buffer), sizeof(buffer));
    SRSECURITY_LOG_ERROR("pcre compile error: {}", buffer);
    return;
  }
}

void PatternList::add(const std::string& pattern, bool case_less, uint64_t id) {
  if (pattern_map_.find(id) != pattern_map_.end()) [[unlikely]] {
    SRSECURITY_LOG_ERROR("add pattern failure! there has same id: {} {}", id, pattern);
    return;
  }

  pattern_map_.emplace(std::piecewise_construct, std::forward_as_tuple(id),
                       std::forward_as_tuple(pattern, case_less));
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