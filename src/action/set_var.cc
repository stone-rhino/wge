#include "set_var.h"

#include <charconv>

#include <assert.h>

namespace SrSecurity {
namespace Action {
SetVar::SetVar(std::string&& name, std::string&& value, EvaluateType type)
    : name_(std::move(name)), value_(std::move(value)), type_(type) {
  // The variable name is case insensitive
  std::transform(name_.begin(), name_.end(), name_.begin(),
                 [](unsigned char c) { return std::tolower(c); });
}

SetVar::SetVar(std::string&& name, std::shared_ptr<Macro::MacroBase> macro, EvaluateType type)
    : name_(std::move(name)), macro_(macro), type_(type) {
  // The variable name is case insensitive
  std::transform(name_.begin(), name_.end(), name_.begin(),
                 [](unsigned char c) { return std::tolower(c); });
}

void SetVar::evaluate(Transaction& t) const {
  switch (type_) {
  case EvaluateType::Create:
    t.createVariable(std::string(name_));
    break;
  case EvaluateType::CreateAndInit:
    if (macro_) {
      std::string_view value = macro_->evaluate(t);
      assert(!value.empty());
      if (!value.empty()) {
        t.createVariable(std::string(name_), std::string(value));
      }
    } else {
      t.createVariable(std::string(name_), ::atoll(value_.c_str()));
    }
    break;
  case EvaluateType::Remove:
    t.removeVariable(name_);
    break;
  case EvaluateType::Increase:
    if (macro_) {
      std::string_view value = macro_->evaluate(t);
      assert(!value.empty());
      if (!value.empty()) {
        int64_t v;
        std::from_chars(value.data(), value.data() + value.size(), v);
        t.increaseVariable(name_, v);
      }
    } else {
      t.increaseVariable(name_, ::atoll(value_.c_str()));
    }
    break;
  case EvaluateType::Decrease:
    if (macro_) {
      std::string_view value = macro_->evaluate(t);
      assert(!value.empty());
      if (!value.empty()) {
        int64_t v;
        std::from_chars(value.data(), value.data() + value.size(), v);
        t.increaseVariable(name_, -v);
      }
    } else {
      t.increaseVariable(name_, -::atoll(value_.c_str()));
    }
    break;
  default:
    break;
  }
}
} // namespace Action
} // namespace SrSecurity