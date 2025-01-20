#pragma once

#include "operator_base.h"

#include "../common/util.h"

namespace SrSecurity {
namespace Operator {
class ContainsWord : public OperatorBase {
  DECLARE_OPERATOR_NAME(containsWord);

public:
  ContainsWord(std::string&& operator_value) : OperatorBase(std::move(operator_value)) {}

public:
  void preCompile() override {
    regex_expr_ = R"EOF(\b)EOF" + Util::regexEscape(operator_value_) + R"EOF(\b)EOF";
  }
};
} // namespace Operator
} // namespace SrSecurity