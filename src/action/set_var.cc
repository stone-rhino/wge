#include "set_var.h"

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

void SetVar::evaluate(Transaction& t) {
  switch (type_) {
  case EvaluateType::Create:
    t.createVariable(std::move(name_));
    break;
  case EvaluateType::CreateAndInit:
    if (macro_) {
      std::string* value = macro_->evaluate(t);
      assert(value);
      if (value) {
        t.createVariable(std::move(name_), std::string(*value));
      }
    } else {
      t.createVariable(std::move(name_), ::atoll(value_.c_str()));
    }
    break;
  case EvaluateType::Remove:
    t.removeVariable(name_);
    break;
  case EvaluateType::Increase:
    if (macro_) {
      std::string* value = macro_->evaluate(t);
      assert(value);
      if (value) {
        t.increaseVariable(name_, ::atoll(value->c_str()));
      }
    } else {
      t.increaseVariable(name_, ::atoll(value_.c_str()));
    }
    break;
  case EvaluateType::Decrease:
    if (macro_) {
      std::string* value = macro_->evaluate(t);
      assert(value);
      if (value) {
        t.increaseVariable(name_, -::atoll(value->c_str()));
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