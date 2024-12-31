#pragma once

#include "action_base.h"

namespace SrSecurity {
namespace Action {
class Msg : public ActionBase {
public:
  Msg(std::string&& name, std::string&& value) : ActionBase(std::move(name), std::move(value)) {}
};
} // namespace Action
} // namespace SrSecurity