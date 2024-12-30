#pragma once

#include <string>
#include <string_view>

#include "variable_base.h"

namespace SrSecurity {
namespace Variable {
class ArgsPost : public VariableBase {
  friend class Args;

public:
  ArgsPost(std::string&& full_name, bool is_not, bool is_counter)
      : VariableBase(std::move(full_name), is_not, is_counter) {}

public:
  void preCompile() override { regex_expr_.req_body_ = regex_; }

private:
  static constexpr char regex_[] = R"EOF(&?[\w-]+=[\w-]+)EOF";
};
} // namespace Variable
} // namespace SrSecurity