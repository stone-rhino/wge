#pragma once

#include "action_base.h"

#include "../macro/macro_base.h"

namespace SrSecurity {
namespace Action {
class InitCol : public ActionBase {
public:
  InitCol(std::string&& name, std::string&& value);

public:
  void evaluate(Transaction& t) const override;

private:
  std::string name_;
  std::string value_;
};
} // namespace Action
} // namespace SrSecurity