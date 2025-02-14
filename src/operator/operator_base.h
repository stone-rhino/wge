#pragma once

#include <string>

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
  OperatorBase(std::string&& literal_value) : literal_value_(std::move(literal_value)) {}
  virtual ~OperatorBase() = default;

public:
  const std::string& literalValue() const { return literal_value_; }

public:
  virtual bool evaluate(Transaction& t, const std::string& value) const = 0;
  virtual const char* name() const = 0;

protected:
  std::string literal_value_;
};

} // namespace Operator
} // namespace SrSecurity