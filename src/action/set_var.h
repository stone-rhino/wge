#pragma once

#include <regex>
#include <string>

#include "action_base.h"

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
public:
  SetVar(std::string&& value) : ActionBase(std::move(value)) {}

public:
  void evaluate(Transaction* t) override;

private:
  static const std::regex pattern_;
};
} // namespace Action
} // namespace SrSecurity