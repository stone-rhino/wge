#pragma once

#include <memory>
#include <string>

#include "action_base.h"

#include "../macro/macro_base.h"

namespace SrSecurity {
namespace Action {
/**
 * Special-purpose action that initializes the RESOURCE collection using a key provided as
 * parameter.
 */
class SetRsc : public ActionBase {
public:
  SetRsc(std::string&& value);
  SetRsc(std::shared_ptr<Macro::MacroBase> macro);

public:
  void evaluate(Transaction& t) const override;

private:
  std::string value_;
  std::shared_ptr<Macro::MacroBase> macro_;
};
} // namespace Action
} // namespace SrSecurity