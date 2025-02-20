#include "set_env.h"

#include <stdlib.h>

#include "../common/assert.h"

namespace SrSecurity {
namespace Action {
SetEnv::SetEnv(std::string&& name, std::string&& value)
    : name_(std::move(name)), value_(std::move(value)) {
  // The variable name is case insensitive
  std::transform(name_.begin(), name_.end(), name_.begin(),
                 [](unsigned char c) { return std::tolower(c); });
}

SetEnv::SetEnv(std::string&& name, std::shared_ptr<Macro::MacroBase> macro)
    : name_(std::move(name)), macro_(macro) {
  // The variable name is case insensitive
  std::transform(name_.begin(), name_.end(), name_.begin(),
                 [](unsigned char c) { return std::tolower(c); });
}

void SetEnv::evaluate(Transaction& t) const {
  if (macro_) {
    Common::Variant value = macro_->evaluate(t);
    if (IS_INT_VARIANT(value)) {
      ::setenv(name_.c_str(), std::to_string(std::get<int>(value)).c_str(), 1);
    } else if (IS_STRING_VARIANT(value)) {
      ::setenv(name_.c_str(), std::get<std::string>(value).c_str(), 1);
    } else if (IS_STRING_VIEW_VARIANT(value)) {
      std::string value_str(std::get<std::string_view>(value));
      ::setenv(name_.c_str(), value_str.c_str(), 1);
    } else {
      UNREACHABLE();
    }
  } else {
    ::setenv(name_.c_str(), value_.c_str(), 1);
  }
}
} // namespace Action
} // namespace SrSecurity