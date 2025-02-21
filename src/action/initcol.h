#pragma once

#include "action_base.h"

#include "../macro/macro_base.h"

namespace SrSecurity {
namespace Action {
class InitCol : public ActionBase {
  DECLARE_ACTION_NAME(initcol);

public:
  InitCol(std::string&& key, std::string&& value);

public:
  void evaluate(Transaction& t) const override;

private:
  std::string key_;
  std::string value_;
};
} // namespace Action
} // namespace SrSecurity