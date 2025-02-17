#pragma once

#include "action_base.h"

#include "../macro/macro_base.h"

namespace SrSecurity {
namespace Action {
/**
 * Creates and updates environment variables that can be accessed by both ModSecurity and the web
 * server.
 */
class T : public ActionBase {
public:
  T();

public:
  void evaluate(Transaction& t) const override;
};
} // namespace Action
} // namespace SrSecurity