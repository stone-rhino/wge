#pragma once

#include <memory>
#include <string>

#include "action_base.h"

#include "../macro/macro_base.h"

namespace SrSecurity {
namespace Action {
/**
 * Special-purpose action that initializes the USER collection using the username provided as
 * parameter.
 */
class SetUid : public ActionBase {
public:
  SetUid(std::string&& value);
  SetUid(std::shared_ptr<Macro::MacroBase> macro);

public:
  void evaluate(Transaction& t) const override;

private:
  std::string value_;
  std::shared_ptr<Macro::MacroBase> macro_;
};
} // namespace Action
} // namespace SrSecurity