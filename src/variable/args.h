#pragma once

#include <string>
#include <string_view>

#include "args_get.h"
#include "args_post.h"
#include "variable_base.h"

namespace SrSecurity {
namespace Variable {
class Args : public VariableBase {
  DECLARE_VIRABLE_NAME(ARGS);

public:
  Args(std::string&& sub_name, bool is_not, bool is_counter)
      : VariableBase(std::move(sub_name), is_not, is_counter) {}

public:
  void preCompile() override {
    regex_expr_.req_line_ = ArgsGet::regex_;
    regex_expr_.req_body_ = ArgsPost::regex_;
  }
};
} // namespace Variable
} // namespace SrSecurity