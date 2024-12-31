#pragma once

#include "action_base.h"

namespace SrSecurity {
namespace Action {
class Tag : public ActionBase {
public:
  Tag(std::string&& name, std::string&& value) : ActionBase(std::move(name), std::move(value)) {}
};
} // namespace Action
} // namespace SrSecurity