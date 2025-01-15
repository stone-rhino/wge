#pragma once

#include "action_base.h"

#include "../macro/macro_base.h"

namespace SrSecurity {
namespace Action {
// Creates and updates environment variables that can be accessed by both ModSecurity and the web
// server.
class SetEnv : public ActionBase {
public:
  SetEnv(std::string&& name, std::string&& value);
  SetEnv(std::string&& name, std::shared_ptr<Macro::MacroBase> macro);

public:
  void evaluate(Transaction& t) override;

private:
  std::string name_;
  std::string value_;
  std::shared_ptr<Macro::MacroBase> macro_;
};
} // namespace Action
} // namespace SrSecurity