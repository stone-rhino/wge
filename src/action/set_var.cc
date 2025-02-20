#include "set_var.h"

#include <charconv>

#include <assert.h>

namespace SrSecurity {
namespace Action {
SetVar::SetVar(std::string&& name, Common::Variant&& value, EvaluateType type)
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
    t.createVariable(std::string(name_), 1);
    break;
  case EvaluateType::CreateAndInit:
    if (macro_) {
      Common::Variant value = macro_->evaluate(t);
      t.createVariable(std::string(name_), std::move(value));
    } else {
      t.createVariable(std::string(name_), Common::Variant(value_));
    }
    break;
  case EvaluateType::Remove:
    t.removeVariable(name_);
    break;
  case EvaluateType::Increase:
    if (macro_) {
      int value = std::get<int>(macro_->evaluate(t));
      t.increaseVariable(name_, value);
    } else {
      t.increaseVariable(name_, std::get<int>(value_));
    }
    break;
  case EvaluateType::Decrease:
    if (macro_) {
      int value = std::get<int>(macro_->evaluate(t));
      t.increaseVariable(name_, -value);
    } else {
      t.increaseVariable(name_, -std::get<int>(value_));
    }
    break;
  default:
    break;
  }
}
} // namespace Action
} // namespace SrSecurity