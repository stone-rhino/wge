#pragma once

#include <memory>
#include <regex>
#include <string>

#include "macro_base.h"

namespace SrSecurity {
namespace Macro {
class MacroFactory {
public:
  static std::shared_ptr<MacroBase> createMacro(const std::string& macro);

private:
  static std::regex pattern_;
};

} // namespace Macro
} // namespace SrSecurity