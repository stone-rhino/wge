#pragma once

#include <regex>
#include <string>

#include "../transaction.h"

namespace SrSecurity {
namespace Action {

// Creates, removes, or updates a variable. Variable names are case-insensitive.
class SetVar {
public:
  SetVar(std::string&& value) : value_(std::move(value)) {}

public:
  void evaluate(Transaction* t);

private:
  std::string value_;
  static const std::regex pattern_;
};
} // namespace Action
} // namespace SrSecurity