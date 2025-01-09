#include "set_var.h"

#include <assert.h>

#include "../macro/factory.h"

namespace SrSecurity {
namespace Action {
/**
 *E.g. "TX.score=10"
 *         ╰-┬-╯  |
 *  variable name |
 *            variable value
 *
 *E.g. "!TX.auth_attempt"
 *      |   ╰----┬-----╯
 * remove flag   |
 *         variable name
 *
 */
const std::regex SetVar::pattern_{R"(^(!)?(?:TX|tx)\.(\w+)(?:=(\+|-)?(?:([^\d][\w%\{\}\.]+)|(\d+)))?)"};

void SetVar::evaluate(Transaction* t) {
  std::smatch sm;
  bool is_not = false;
  std::string increase;
  std::string var_name;
  std::string var_value_str;
  uint32_t var_value_num = 0;
  if (std::regex_match(value_, sm, pattern_)) {
    is_not = !sm.str(1).empty();
    var_name = sm.str(2);
    increase = sm.str(3);
    var_value_str = sm.str(4);
    if (var_value_str.empty()) {
      var_value_num = ::atol(sm.str(5).c_str());
      if (increase == "-") {
        var_value_num = -var_value_num;
      }
    }
  }

  // The variable name is case insensitive
  std::transform(var_name.begin(), var_name.end(), var_name.begin(),
                 [](unsigned char c) { return std::tolower(c); });

  if (is_not) {
    t->removeVariable(var_name);
    return;
  }

  if (!increase.empty()) {
    t->increaseVariable(var_name, var_value_num);
    return;
  }

  if (var_value_str.empty()) {
    if (var_value_num == 0) {
      t->createVariable(std::move(var_name));
    } else {
      t->createVariable(std::move(var_name), var_value_num);
    }
  } else {
    if (Macro::MacroBase::isMacro(var_value_str)) {
      auto macro = Macro::Factory::instance(std::move(var_value_str));
      assert(macro);
      if (macro) {
        std::string* value = macro->evaluate(t);
        assert(value);
        if (value) {
          t->createVariable(std::move(var_name), std::string(*value));
        }
      }
    } else {
      t->createVariable(std::move(var_name), std::move(var_value_str));
    }
  }
}
} // namespace Action
} // namespace SrSecurity