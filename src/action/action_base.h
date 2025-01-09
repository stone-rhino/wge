#pragma once

#include <string>

#include "../transaction.h"

namespace SrSecurity {
namespace Action {
class ActionBase {
public:
  ActionBase(std::string&& value) : value_(std::move(value)) {}

public:
  virtual void evaluate(Transaction* t) = 0;

protected:
  std::string value_;
};
} // namespace Action
} // namespace SrSecurity