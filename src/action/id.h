#pragma once

#include "action_base.h"

namespace SrSecurity {
namespace Action {
class Id : public ActionBase {
public:
  Id(std::string&& name, std::string&& value) : ActionBase(std::move(name), std::move(value)) {}
};
} // namespace Action
} // namespace SrSecurity