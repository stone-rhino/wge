#pragma once

#include <memory>
#include <string>

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
  SetVar(std::string&& key, Common::Variant&& value, EvaluateType type);
  SetVar(std::string&& key, const std::shared_ptr<Macro::MacroBase> value, EvaluateType type);
  SetVar(const std::shared_ptr<Macro::MacroBase> key, Common::Variant&& value, EvaluateType type);
  SetVar(const std::shared_ptr<Macro::MacroBase> key, const std::shared_ptr<Macro::MacroBase> value,
         EvaluateType type);

public:
  void evaluate(Transaction& t) const override;

public:
  const std::string& key() const { return key_; }
  const Common::Variant& value() const { return value_; }

private:
  std::string key_;
  const Common::Variant value_;
  EvaluateType type_;
  const std::shared_ptr<Macro::MacroBase> key_macro_;
  const std::shared_ptr<Macro::MacroBase> value_macro_;
};
} // namespace Action
} // namespace SrSecurity