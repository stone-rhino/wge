#pragma once
#include <string>

namespace SrSecurity {
namespace Variable {
class VariableBase {
public:
  virtual const std::string& name() const = 0;
};
} // namespace Variable
} // namespace SrSecurity