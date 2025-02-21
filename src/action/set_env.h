#pragma once

#include "action_base.h"

#include "../macro/macro_base.h"

namespace SrSecurity {
namespace Action {
/**
 * Creates and updates environment variables that can be accessed by both ModSecurity and the web
 * server.
 */
class SetEnv : public ActionBase {
  DECLARE_ACTION_NAME(setenv);

public:
  SetEnv(std::string&& key, std::string&& value);
  SetEnv(std::string&& key, std::shared_ptr<Macro::MacroBase> macro);

public:
  void evaluate(Transaction& t) const override;

private:
  std::string key_;
  std::string value_;
  std::shared_ptr<Macro::MacroBase> macro_;
};
} // namespace Action
} // namespace SrSecurity