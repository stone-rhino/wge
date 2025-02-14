#pragma once

#include <string>

#include "../variable/variable_base.h"

#define DECLARE_OPERATOR_NAME(n)                                                                   \
public:                                                                                            \
  const char* name() const override { return name_; }                                              \
                                                                                                   \
private:                                                                                           \
  static constexpr char name_[] = #n;

namespace SrSecurity {
namespace Operator {

class OperatorBase {
public:
  OperatorBase(std::string&& operator_value) : operator_value_(std::move(operator_value)) {}

public:
  const std::string& value() const { return operator_value_; }
  const std::string& regexExpr() const { return regex_expr_; }

public:
  virtual bool evaluate(Transaction& t, Variable::VariableBase* variable) const { return false; }
  virtual void preCompile() {}
  virtual const char* name() const = 0;

protected:
  std::string operator_value_;
  std::string regex_expr_;
};

} // namespace Operator
} // namespace SrSecurity