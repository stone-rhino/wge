#include "set_var.h"

#include "../common/assert.h"
#include "../common/log.h"

namespace SrSecurity {
namespace Action {
SetVar::SetVar(std::string&& key, Common::Variant&& value, EvaluateType type)
    : key_(std::move(key)), value_(std::move(value)), type_(type) {
  // The variable name is case insensitive
  std::transform(key_.begin(), key_.end(), key_.begin(),
                 [](unsigned char c) { return std::tolower(c); });
}

SetVar::SetVar(std::string&& key, std::shared_ptr<Macro::MacroBase> macro, EvaluateType type)
    : key_(std::move(key)), macro_(macro), type_(type) {
  // The variable name is case insensitive
  std::transform(key_.begin(), key_.end(), key_.begin(),
                 [](unsigned char c) { return std::tolower(c); });
}

void SetVar::evaluate(Transaction& t) const {
  switch (type_) {
  case EvaluateType::Create:
    SRSECURITY_LOG_TRACE("setvar(Create): {}=1", key_);
    t.createVariable(std::string(key_), 1);
    break;
  [[likely]] case EvaluateType::CreateAndInit:
    if (macro_) {
      Common::Variant value = macro_->evaluate(t);
      SRSECURITY_LOG_TRACE("setvar(CreateAndInit): {}={}", key_, VISTIT_VARIANT_AS_STRING(value));
      t.createVariable(std::string(key_), std::move(value));
    } else {
      SRSECURITY_LOG_TRACE("setvar(CreateAndInit): {}={}", key_, VISTIT_VARIANT_AS_STRING(value_));
      t.createVariable(std::string(key_), Common::Variant(value_));
    }
    break;
  case EvaluateType::Remove:
    SRSECURITY_LOG_TRACE("setvar(Remove): {}", key_);
    t.removeVariable(key_);
    break;
  case EvaluateType::Increase:
    if (macro_) {
      int value = std::get<int>(macro_->evaluate(t));
      SRSECURITY_LOG_TRACE("setvar(Increase): {}+={}", key_, value);
      t.increaseVariable(key_, value);
    } else {
      SRSECURITY_LOG_TRACE("setvar(Increase): {}+={}", key_, std::get<int>(value_));
      t.increaseVariable(key_, std::get<int>(value_));
    }
    break;
  case EvaluateType::Decrease:
    if (macro_) {
      int value = std::get<int>(macro_->evaluate(t));
      SRSECURITY_LOG_TRACE("setvar(Decrease): {}-={}", key_, value);
      t.increaseVariable(key_, -value);
    } else {
      SRSECURITY_LOG_TRACE("setvar(Decrease): {}-={}", key_, std::get<int>(value_));
      t.increaseVariable(key_, -std::get<int>(value_));
    }
    break;
  default:
    UNREACHABLE();
    break;
  }
}
} // namespace Action
} // namespace SrSecurity