#pragma once

#include <string>

#include "../transaction.h"

namespace SrSecurity {
namespace Macro {
/**
 * Macros allow for using place holders in rules that will be expanded out to their values at
 * runtime. Currently only variable expansion is supported, however more options may be added in
 * future versions of ModSecurity.
 * Format:
 * %{VARIABLE}
 * %{COLLECTION.VARIABLE}
 */
class MacroBase {
public:
  MacroBase(const std::string& value) { parseMacroName(value); }

public:
  virtual std::string* evaluate(Transaction* t) = 0;

public:
  static bool isMacro(const std::string& value) {
    return value.starts_with("%{") && value.ends_with("}");
  }

private:
  void parseMacroName(const std::string& value) {
    if (isMacro(value)) {
      macro_name_ = value.substr(2);
      macro_name_.erase(std::prev(macro_name_.end()));
    }
  }

protected:
  std::string macro_name_;
};
} // namespace Macro
} // namespace SrSecurity