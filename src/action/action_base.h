#pragma once

#include <string>

namespace SrSecurity {
namespace Action {
class ActionBase {
public:
  ActionBase(std::string&& name, std::string&& value) : name_(name), value_(value) {}

public:
  const std::string& name() const { return name_; }
  const std::string& value() const { return value_; }

private:
  std::string name_;
  std::string value_;
};
} // namespace Action
} // namespace SrSecurity