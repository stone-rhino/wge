#include "set_env.h"

#include <stdlib.h>

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
    std::string_view value = macro_->evaluate(t);
    if (!value.empty()) {
      ::setenv(name_.c_str(), std::string(value).c_str(), 1);
    }
  } else {
    ::setenv(name_.c_str(), value_.c_str(), 1);
  }
}
} // namespace Action
} // namespace SrSecurity