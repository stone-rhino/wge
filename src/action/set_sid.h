#pragma once

#include <memory>
#include <string>

#include "action_base.h"

#include "../macro/macro_base.h"

namespace SrSecurity {
namespace Action {
/**
 * Special-purpose action that initializes the SESSION collection using the session token provided
 * as parameter.
 */
class SetSid : public ActionBase {
  DECLARE_ACTION_NAME(setsid);

public:
  SetSid(std::string&& value);
  SetSid(std::shared_ptr<Macro::MacroBase> macro);

public:
  void evaluate(Transaction& t) const override;

private:
  std::string value_;
  std::shared_ptr<Macro::MacroBase> macro_;
};
} // namespace Action
} // namespace SrSecurity