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
#pragma once

#include <memory>
#include <optional>
#include <string>
#include <unordered_map>

#include "action_base.h"

#include "../common/variant.h"
#include "../macro/macro_base.h"

namespace SrSecurity {
namespace Action {
/**
 * Creates, removes, or updates a variable. Variable names are case-insensitive.
 * Examples:
 * To create a variable and set its value to 1 (usually used for setting flags), use:
 * setvar:TX.score
 * To create a variable and initialize it at the same time, use: setvar:TX.score=10
 * To remove a variable, prefix the name with an exclamation mark: setvar:!TX.score
 * To increase or decrease variable value, use + and - characters in front of a numerical value:
 * setvar:TX.score=+5
 */
class SetVar : public ActionBase {
  DECLARE_ACTION_NAME(setvar);

public:
  enum class EvaluateType { Create, CreateAndInit, Remove, Increase, Decrease };

public:
  SetVar(std::string&& key, size_t index, Common::Variant&& value, EvaluateType type);
  SetVar(std::string&& key, size_t index, const std::shared_ptr<Macro::MacroBase> value,
         EvaluateType type);
  SetVar(const std::shared_ptr<Macro::MacroBase> key, Common::Variant&& value, EvaluateType type);
  SetVar(const std::shared_ptr<Macro::MacroBase> key, const std::shared_ptr<Macro::MacroBase> value,
         EvaluateType type);

public:
  void evaluate(Transaction& t) const override;

public:
  const std::string& key() const { return key_; }
  const Common::Variant& value() const { return value_; }
  size_t index() const { return index_; }

private:
  std::string key_;
  size_t index_;
  const Common::Variant value_;
  const std::string value_buffer_;
  EvaluateType type_;
  const std::shared_ptr<Macro::MacroBase> key_macro_;
  const std::shared_ptr<Macro::MacroBase> value_macro_;
};
} // namespace Action
} // namespace SrSecurity