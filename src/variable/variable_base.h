#pragma once

#include <string>
#include <string_view>

#include "../http_extractor.h"

namespace SrSecurity {
namespace Variable {

/**
 * Base class of variable
 */
class VariableBase {
public:
  struct RegexExpression {
    std::string req_line_;
    std::string req_headers_;
    std::string req_body_;
    std::string resp_line_;
    std::string resp_headers_;
    std::string resp_body_;
  };

public:
  VariableBase(std::string&& full_name, bool is_not, bool is_counter)
      : full_name_(std::move(full_name)), is_not_(is_not), is_counter_(is_counter) {
    initName();
  }

public:
  virtual void preCompile() = 0;

public:
  const std::string& fullName() const { return full_name_; }
  const std::string_view& mainName() const { return main_name_; }
  const std::string_view& subName() const { return sub_name_; }
  bool isNot() const { return is_not_; }
  bool isCounter() const { return is_counter_; }
  const RegexExpression& regexExpr() const { return regex_expr_; }

private:
  void initName() {
    std::string_view full_name(full_name_);
    auto pos = full_name.find(':');
    if (pos != full_name.npos) {
      main_name_ = full_name.substr(0, pos);
      sub_name_ = full_name.substr(pos + 1);
    } else {
      main_name_ = full_name;
    }
  }

protected:
  std::string full_name_;
  std::string_view main_name_;
  std::string_view sub_name_;
  bool is_not_;
  bool is_counter_;
  RegexExpression regex_expr_;
};
} // namespace Variable
} // namespace SrSecurity