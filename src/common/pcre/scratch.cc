#include "scratch.h"

#ifndef PCRE2_STATIC
#define PCRE2_STATIC
#endif

#ifndef PCRE2_CODE_UNIT_WIDTH
#define PCRE2_CODE_UNIT_WIDTH 8
#else
#error PCRE2_CODE_UNIT_WIDTH was defined!
#endif

#include <pcre2.h>

namespace SrSecurity {
namespace Common {
namespace Pcre {
Scratch::Scratch(int result_count) { scratch_ = pcre2_match_data_create(result_count, nullptr); }

Scratch::~Scratch() {
  if (scratch_) {
    pcre2_match_data_free(reinterpret_cast<pcre2_match_data*>(scratch_));
    scratch_ = nullptr;
  }
}
} // namespace Pcre
} // namespace Common
} // namespace SrSecurity