#include "set_var.h"

#include "../common/assert.h"
#include "../common/log.h"

namespace SrSecurity {
namespace Action {
SetVar::SetVar(std::string&& key, size_t index, Common::Variant&& value, EvaluateType type)
    : key_(std::move(key)), index_(index), value_(std::move(value)), type_(type) {
  // Holds the string value of the variant
  if (IS_STRING_VIEW_VARIANT(value_)) {
    const_cast<std::string&>(value_buffer_) = std::get<std::string_view>(value_);
    const_cast<Common::Variant&>(value_) = value_buffer_;
  }
}

SetVar::SetVar(std::string&& key, size_t index, const std::shared_ptr<Macro::MacroBase> value,
               EvaluateType type)
    : key_(std::move(key)), index_(index), value_macro_(value), type_(type) {
  // Holds the string value of the variant
  if (IS_STRING_VIEW_VARIANT(value_)) {
    const_cast<std::string&>(value_buffer_) = std::get<std::string_view>(value_);
    const_cast<Common::Variant&>(value_) = value_buffer_;
  }
}

SetVar::SetVar(const std::shared_ptr<Macro::MacroBase> key, Common::Variant&& value,
               EvaluateType type)
    : key_macro_(key), value_(std::move(value)), type_(type) {
  // Holds the string value of the variant
  if (IS_STRING_VIEW_VARIANT(value_)) {
    const_cast<std::string&>(value_buffer_) = std::get<std::string_view>(value_);
    const_cast<Common::Variant&>(value_) = value_buffer_;
  }
}

SetVar::SetVar(const std::shared_ptr<Macro::MacroBase> key,
               const std::shared_ptr<Macro::MacroBase> value, EvaluateType type)
    : key_macro_(key), value_macro_(value), type_(type) {
  // Holds the string value of the variant
  if (IS_STRING_VIEW_VARIANT(value_)) {
    const_cast<std::string&>(value_buffer_) = std::get<std::string_view>(value_);
    const_cast<Common::Variant&>(value_) = value_buffer_;
  }
}

void SetVar::evaluate(Transaction& t) const {
  switch (type_) {
  case EvaluateType::Create: {
    if (key_macro_) [[unlikely]] {
      std::string_view key = std::get<std::string_view>(key_macro_->evaluate(t));
      SRSECURITY_LOG_TRACE("setvar(Create): tx.{}=1", key);
      t.setVariable({key.data(), key.size()}, 1);
    } else {
      SRSECURITY_LOG_TRACE("setvar(Create): tx.{}[{}]=1", key_, index_);
      t.setVariable(index_, 1);
    }

  } break;
  [[likely]] case EvaluateType::CreateAndInit: {
    if (key_macro_) [[unlikely]] {
      std::string_view key = std::get<std::string_view>(key_macro_->evaluate(t));
      if (value_macro_) [[unlikely]] {
        Common::Variant value = value_macro_->evaluate(t);
        SRSECURITY_LOG_TRACE("setvar(CreateAndInit): tx.{}={}", key,
                             VISTIT_VARIANT_AS_STRING(value));
        t.setVariable({key.data(), key.size()}, std::move(value));
      } else {
        SRSECURITY_LOG_TRACE("setvar(CreateAndInit): tx.{}={}", key,
                             VISTIT_VARIANT_AS_STRING(value_));
        t.setVariable({key.data(), key.size()}, Common::Variant(value_));
      }
    } else {
      if (value_macro_) [[unlikely]] {
        Common::Variant value = value_macro_->evaluate(t);
        SRSECURITY_LOG_TRACE("setvar(CreateAndInit): tx.{}[{}]={}", key_, index_,
                             VISTIT_VARIANT_AS_STRING(value));
        t.setVariable(index_, std::move(value));
      } else {
        SRSECURITY_LOG_TRACE("setvar(CreateAndInit): tx.{}[{}]={}", key_, index_,
                             VISTIT_VARIANT_AS_STRING(value_));
        t.setVariable(index_, Common::Variant(value_));
      }
    }
  } break;
  case EvaluateType::Remove: {
    if (key_macro_) [[unlikely]] {
      std::string_view key = std::get<std::string_view>(key_macro_->evaluate(t));
      SRSECURITY_LOG_TRACE("setvar(Remove): tx.{}", key);
      t.removeVariable({key.data(), key.size()});
    } else {
      SRSECURITY_LOG_TRACE("setvar(Remove): tx.{}[{}]", key_, index_);
      t.removeVariable(index_);
    }

  } break;
  case EvaluateType::Increase: {
    if (key_macro_) [[unlikely]] {
      std::string_view key = std::get<std::string_view>(key_macro_->evaluate(t));
      if (value_macro_) {
        int value = std::get<int>(value_macro_->evaluate(t));
        SRSECURITY_LOG_TRACE("setvar(Increase): tx.{}+={}", key, value);
        t.increaseVariable({key.data(), key.size()}, value);
      } else {
        SRSECURITY_LOG_TRACE("setvar(Increase): tx.{}+={}", key, std::get<int>(value_));
        t.increaseVariable({key.data(), key.size()}, std::get<int>(value_));
      }
    } else {
      if (value_macro_) {
        int value = std::get<int>(value_macro_->evaluate(t));
        SRSECURITY_LOG_TRACE("setvar(Increase): tx.{}[{}]+={}", key_, index_, value);
        t.increaseVariable(index_, value);
      } else {
        SRSECURITY_LOG_TRACE("setvar(Increase): tx.{}[{}]+={}", key_, index_,
                             std::get<int>(value_));
        t.increaseVariable(index_, std::get<int>(value_));
      }
    }

  } break;
  case EvaluateType::Decrease: {
    if (key_macro_) [[unlikely]] {
      std::string_view key = std::get<std::string_view>(key_macro_->evaluate(t));
      if (value_macro_) {
        int value = std::get<int>(value_macro_->evaluate(t));
        SRSECURITY_LOG_TRACE("setvar(Decrease): tx.{}-={}", key, value);
        t.increaseVariable({key.data(), key.size()}, -value);
      } else {
        SRSECURITY_LOG_TRACE("setvar(Decrease): tx.{}-={}", key, std::get<int>(value_));
        t.increaseVariable({key.data(), key.size()}, -std::get<int>(value_));
      }
    } else {
      if (value_macro_) {
        int value = std::get<int>(value_macro_->evaluate(t));
        SRSECURITY_LOG_TRACE("setvar(Decrease): tx.{}[{}]-={}", key_, index_, value);
        t.increaseVariable(index_, -value);
      } else {
        SRSECURITY_LOG_TRACE("setvar(Decrease): tx.{}[{}]-={}", key_, index_,
                             std::get<int>(value_));
        t.increaseVariable(index_, -std::get<int>(value_));
      }
    }
  } break;
  default:
    UNREACHABLE();
    break;
  }
}
} // namespace Action
} // namespace SrSecurity