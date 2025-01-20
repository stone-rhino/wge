#pragma once

#include <string>
#include <string_view>

#include "variable_base.h"

namespace SrSecurity {
namespace Variable {
class ArgsGet : public VariableBase {
  friend class Args;
  DECLARE_VIRABLE_NAME(ARGS_GET);

public:
  ArgsGet(std::string&& sub_name, bool is_not, bool is_counter)
      : VariableBase(std::move(sub_name), is_not, is_counter) {}

public:
  void preCompile() override { regex_expr_.req_line_ = regex_; }

private:
  static constexpr char regex_[] = R"EOF([?&][\w-]+=[\w-]+)EOF";
};
} // namespace Variable
} // namespace SrSecurity