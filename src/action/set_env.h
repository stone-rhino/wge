#pragma once

#include "action_base.h"

namespace SrSecurity {
namespace Action {
// Creates and updates environment variables that can be accessed by both ModSecurity and the web
// server.
class SetEnv : public ActionBase {
public:
  SetEnv() {}

public:
  void evaluate(Transaction& t) override;
};
} // namespace Action
} // namespace SrSecurity