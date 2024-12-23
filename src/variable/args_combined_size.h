#pragma once
#include <string>

#include "variable_base.h"

namespace SrSecurity {
namespace Variable {
class ArgsCombinedSize : public VariableBase {
public:
  const std::string& name() const override;
};
} // namespace Variable
} // namespace SrSecurity