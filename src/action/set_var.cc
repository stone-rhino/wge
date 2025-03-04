#include "set_var.h"

#include "../common/assert.h"
#include "../common/log.h"

namespace SrSecurity {
namespace Action {
SetVar::SetVar(std::string&& key, Common::Variant&& value, EvaluateType type)
    : key_(std::move(key)), value_(std::move(value)), type_(type) {}

SetVar::SetVar(std::string&& key, const std::shared_ptr<Macro::MacroBase> value, EvaluateType type)
    : key_(std::move(key)), value_macro_(value), type_(type) {}

SetVar::SetVar(const std::shared_ptr<Macro::MacroBase> key, Common::Variant&& value,
               EvaluateType type)
    : key_macro_(key), value_(std::move(value)), type_(type) {}

SetVar::SetVar(const std::shared_ptr<Macro::MacroBase> key,
               const std::shared_ptr<Macro::MacroBase> value, EvaluateType type)
    : key_macro_(key), value_macro_(value), type_(type) {}

void SetVar::evaluate(Transaction& t) const {
  switch (type_) {
  case EvaluateType::Create: {
    std::string key(key_);
    if (key_macro_) [[unlikely]] {
      key = std::get<std::string>(key_macro_->evaluate(t));
    }
    SRSECURITY_LOG_TRACE("setvar(Create): tx.{}=1", key);
    t.createVariable(std::move(key), 1);
  } break;
  [[likely]] case EvaluateType::CreateAndInit: {
    std::string key(key_);
    if (key_macro_) [[unlikely]] {
      key = std::get<std::string>(key_macro_->evaluate(t));
    }

    if (value_macro_) [[unlikely]] {
      Common::Variant value = value_macro_->evaluate(t);
      SRSECURITY_LOG_TRACE("setvar(CreateAndInit): tx.{}={}", key_,
                           VISTIT_VARIANT_AS_STRING(value));
      t.createVariable(std::move(key), std::move(value));
    } else {
      SRSECURITY_LOG_TRACE("setvar(CreateAndInit): tx.{}={}", key_,
                           VISTIT_VARIANT_AS_STRING(value_));
      t.createVariable(std::move(key), Common::Variant(value_));
    }
  } break;
  case EvaluateType::Remove: {
    std::string key(key_);
    if (key_macro_) [[unlikely]] {
      key = std::get<std::string>(key_macro_->evaluate(t));
    }

    SRSECURITY_LOG_TRACE("setvar(Remove): tx.{}", key);
    t.removeVariable(key);
  } break;
  case EvaluateType::Increase: {
    std::string key;
    const std::string* pk = &key_;
    if (key_macro_) [[unlikely]] {
      key = std::get<std::string>(key_macro_->evaluate(t));
      pk = &key;
    }

    if (value_macro_) {
      int value = std::get<int>(value_macro_->evaluate(t));
      SRSECURITY_LOG_TRACE("setvar(Increase): tx.{}+={}", key_, value);
      t.increaseVariable(*pk, value);
    } else {
      SRSECURITY_LOG_TRACE("setvar(Increase): tx.{}+={}", key_, std::get<int>(value_));
      t.increaseVariable(*pk, std::get<int>(value_));
    }
  } break;
  case EvaluateType::Decrease: {
    std::string key;
    const std::string* pk = &key_;
    if (key_macro_) [[unlikely]] {
      key = std::get<std::string>(key_macro_->evaluate(t));
      pk = &key;
    }

    if (value_macro_) {
      int value = std::get<int>(value_macro_->evaluate(t));
      SRSECURITY_LOG_TRACE("setvar(Decrease): tx.{}-={}", key_, value);
      t.increaseVariable(*pk, -value);
    } else {
      SRSECURITY_LOG_TRACE("setvar(Decrease): tx.{}-={}", key_, std::get<int>(value_));
      t.increaseVariable(*pk, -std::get<int>(value_));
    }
  } break;
  default:
    UNREACHABLE();
    break;
  }
}
} // namespace Action
} // namespace SrSecurity