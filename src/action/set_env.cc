#include "set_env.h"

#include <stdlib.h>

#include "../common/assert.h"

namespace SrSecurity {
namespace Action {
SetEnv::SetEnv(std::string&& key, std::string&& value)
    : key_(std::move(key)), value_(std::move(value)) {
  // The variable name is case insensitive
  std::transform(key_.begin(), key_.end(), key_.begin(),
                 [](unsigned char c) { return std::tolower(c); });
}

SetEnv::SetEnv(std::string&& key, std::shared_ptr<Macro::MacroBase> macro)
    : key_(std::move(key)), macro_(macro) {
  // The variable name is case insensitive
  std::transform(key_.begin(), key_.end(), key_.begin(),
                 [](unsigned char c) { return std::tolower(c); });
}

void SetEnv::evaluate(Transaction& t) const {
  if (macro_) {
    Common::Variant value = macro_->evaluate(t);
    if (IS_INT_VARIANT(value)) {
      ::setenv(key_.c_str(), std::to_string(std::get<int>(value)).c_str(), 1);
    } else if (IS_STRING_VARIANT(value)) {
      ::setenv(key_.c_str(), std::get<std::string>(value).c_str(), 1);
    } else if (IS_STRING_VIEW_VARIANT(value)) {
      std::string value_str(std::get<std::string_view>(value));
      ::setenv(key_.c_str(), value_str.c_str(), 1);
    } else {
      UNREACHABLE();
    }
  } else {
    ::setenv(key_.c_str(), value_.c_str(), 1);
  }
}
} // namespace Action
} // namespace SrSecurity