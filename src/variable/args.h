#pragma once

#include <string>
#include <string_view>

#include "variable_base.h"

namespace SrSecurity {
namespace Variable {
class Args : public VariableBase {
public:
  const std::string& name() const override { return name_; }
  const std::string_view& subName() const override

private:
  std::string name_;
  std::string_view sub_name_;
};
} // namespace Variable
} // namespace SrSecurity