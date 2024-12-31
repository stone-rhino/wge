#pragma once

#include <string>

namespace SrSecurity {
namespace Operator {

class OperatorBase {
public:
  OperatorBase(std::string&& operator_name, std::string&& operator_value)
      : operator_name_(std::move(operator_name)), operator_value_(std::move(operator_value)) {}

public:
  const std::string& name() const { return operator_name_; }
  const std::string& value() const { return operator_value_; }
  const std::string& regexExpr() const { return regex_expr_; }

public:
  virtual void preCompile() = 0;

protected:
  std::string operator_name_;
  std::string operator_value_;
  std::string regex_expr_;
};

} // namespace Operator
} // namespace SrSecurity