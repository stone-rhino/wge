/**
 * Copyright (c) 2024-2025 Stone Rhino and contributors.
 *
 * MIT License (http://opensource.org/licenses/MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
 * associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
 * NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#include "set_var.h"

#include "../common/assert.h"
#include "../common/log.h"

namespace Wge {
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
    if (key_macro_)
      [[unlikely]] {
        Common::EvaluateResults result;
        key_macro_->evaluate(t, result);
        std::string_view key = std::get<std::string_view>(result.front().variant_);
        WGE_LOG_TRACE("setvar(Create): tx.{}=1", key);
        t.setVariable({key.data(), key.size()}, 1);
      }
    else {
      WGE_LOG_TRACE("setvar(Create): tx.{}[{}]=1", key_, index_);
      t.setVariable(index_, 1);
    }

  } break;
    [[likely]] case EvaluateType::CreateAndInit : {
      if (key_macro_)
        [[unlikely]] {
          Common::EvaluateResults result;
          key_macro_->evaluate(t, result);
          std::string_view key = std::get<std::string_view>(result.front().variant_);
          if (value_macro_)
            [[unlikely]] {
              Common::EvaluateResults result;
              value_macro_->evaluate(t, result);
              WGE_LOG_TRACE("setvar(CreateAndInit): tx.{}={}", key,
                            VISTIT_VARIANT_AS_STRING(result.front().variant_));
              t.setVariable({key.data(), key.size()}, result.front().variant_);
            }
          else {
            WGE_LOG_TRACE("setvar(CreateAndInit): tx.{}={}", key, VISTIT_VARIANT_AS_STRING(value_));
            t.setVariable({key.data(), key.size()}, Common::Variant(value_));
          }
        }
      else {
        if (value_macro_)
          [[unlikely]] {
            Common::EvaluateResults result;
            value_macro_->evaluate(t, result);
            WGE_LOG_TRACE("setvar(CreateAndInit): tx.{}[{}]={}", key_, index_,
                          VISTIT_VARIANT_AS_STRING(result.front().variant_));
            t.setVariable(index_, result.front().variant_);
          }
        else {
          WGE_LOG_TRACE("setvar(CreateAndInit): tx.{}[{}]={}", key_, index_,
                        VISTIT_VARIANT_AS_STRING(value_));
          t.setVariable(index_, value_);
        }
      }
    }
    break;
  case EvaluateType::Remove: {
    if (key_macro_)
      [[unlikely]] {
        Common::EvaluateResults result;
        key_macro_->evaluate(t, result);
        std::string_view key = std::get<std::string_view>(result.front().variant_);
        WGE_LOG_TRACE("setvar(Remove): tx.{}", key);
        t.removeVariable({key.data(), key.size()});
      }
    else {
      WGE_LOG_TRACE("setvar(Remove): tx.{}[{}]", key_, index_);
      t.removeVariable(index_);
    }

  } break;
  case EvaluateType::Increase: {
    if (key_macro_)
      [[unlikely]] {
        Common::EvaluateResults result;
        key_macro_->evaluate(t, result);
        std::string_view key = std::get<std::string_view>(result.front().variant_);
        if (value_macro_) {
          Common::EvaluateResults result;
          value_macro_->evaluate(t, result);
          if (IS_INT_VARIANT(result.front().variant_)) {
            int64_t value = std::get<int64_t>(result.front().variant_);
            WGE_LOG_TRACE("setvar(Increase): tx.{}=+{}", key, value);
            t.increaseVariable({key.data(), key.size()}, value);
          } else {
            WGE_LOG_WARN("setvar(Increase): tx.{}=+{}: value is not an integer, ignored.", key,
                         value_macro_->literalValue());
          }
        } else {
          if (IS_INT_VARIANT(value_)) {
            WGE_LOG_TRACE("setvar(Increase): tx.{}=+{}", key, std::get<int64_t>(value_));
            t.increaseVariable({key.data(), key.size()}, std::get<int64_t>(value_));
          } else {
            WGE_LOG_WARN("setvar(Increase): tx.{}=+{}: value is not an integer, ignored.", key,
                         VISTIT_VARIANT_AS_STRING(value_));
          }
        }
      }
    else {
      if (value_macro_) {
        Common::EvaluateResults result;
        value_macro_->evaluate(t, result);
        if (IS_INT_VARIANT(result.front().variant_)) {
          int64_t value = std::get<int64_t>(result.front().variant_);
          WGE_LOG_TRACE("setvar(Increase): tx.{}[{}]=+{}", key_, index_, value);
          t.increaseVariable(index_, value);
        } else {
          WGE_LOG_WARN("setvar(Increase): tx.{}[{}]=+{}: value is not an integer, ignored.", key_,
                       index_, value_macro_->literalValue());
        }
      } else {
        if (IS_INT_VARIANT(value_)) {
          WGE_LOG_TRACE("setvar(Increase): tx.{}[{}]=+{}", key_, index_, std::get<int64_t>(value_));
          t.increaseVariable(index_, std::get<int64_t>(value_));
        } else {
          WGE_LOG_WARN("setvar(Increase): tx.{}[{}]=+{}: value is not an integer, ignored.", key_,
                       index_, VISTIT_VARIANT_AS_STRING(value_));
        }
      }
    }

  } break;
  case EvaluateType::Decrease: {
    if (key_macro_)
      [[unlikely]] {
        Common::EvaluateResults result;
        key_macro_->evaluate(t, result);
        std::string_view key = std::get<std::string_view>(result.front().variant_);
        if (value_macro_) {
          Common::EvaluateResults result;
          value_macro_->evaluate(t, result);
          if (IS_INT_VARIANT(result.front().variant_)) {
            int64_t value = std::get<int64_t>(result.front().variant_);
            WGE_LOG_TRACE("setvar(Decrease): tx.{}=-{}", key, value);
            t.increaseVariable({key.data(), key.size()}, -value);
          } else {
            WGE_LOG_WARN("setvar(Decrease): tx.{}=-{}: value is not an integer, ignored.", key,
                         value_macro_->literalValue());
          }
        } else {
          if (IS_INT_VARIANT(value_)) {
            WGE_LOG_TRACE("setvar(Decrease): tx.{}=-{}", key, std::get<int64_t>(value_));
            t.increaseVariable({key.data(), key.size()}, -std::get<int64_t>(value_));
          } else {
            WGE_LOG_WARN("setvar(Decrease): tx.{}=-{}: value is not an integer, ignored.", key,
                         VISTIT_VARIANT_AS_STRING(value_));
          }
        }
      }
    else {
      if (value_macro_) {
        Common::EvaluateResults result;
        value_macro_->evaluate(t, result);
        if (IS_INT_VARIANT(result.front().variant_)) {
          int64_t value = std::get<int64_t>(result.front().variant_);
          WGE_LOG_TRACE("setvar(Decrease): tx.{}[{}]-={}", key_, index_, value);
          t.increaseVariable(index_, -value);
        } else {
          WGE_LOG_WARN("setvar(Decrease): tx.{}[{}]=-{}: value is not an integer, ignored.", key_,
                       index_, value_macro_->literalValue());
        }
      } else {
        if (IS_INT_VARIANT(value_)) {
          WGE_LOG_TRACE("setvar(Decrease): tx.{}[{}]-={}", key_, index_, std::get<int64_t>(value_));
          t.increaseVariable(index_, -std::get<int64_t>(value_));
        } else {
          WGE_LOG_WARN("setvar(Decrease): tx.{}[{}]=-{}: value is not an integer, ignored.", key_,
                       index_, VISTIT_VARIANT_AS_STRING(value_));
        }
      }
    }
  } break;
  default:
    UNREACHABLE();
    break;
  }
}
} // namespace Action
} // namespace Wge