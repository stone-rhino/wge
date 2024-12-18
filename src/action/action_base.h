#pragma once
#include <string>

namespace SrSecurity {
namespace Action {
class ActionBase {
public:
  virtual const std::string& name() const = 0;
  virtual const std::string& value() const = 0;

private:
};
} // namespace Action
} // namespace SrSecurity