#include "macro_factory.h"

#include "macro_include.h"

namespace SrSecurity {
namespace Macro {
/**
 * Format is %{collection.variable_name}
 */
std::regex MacroFactory::pattern_(R"(%\{([^%{}.]+)\.([^%{}.]+)\})");

std::shared_ptr<MacroBase> MacroFactory::createMacro(const std::string& macro) {
  std::vector<std::shared_ptr<MacroBase>> macros;
  std::string macro_str = macro;

  std::smatch sm;
  while (std::regex_search(macro_str, sm, pattern_)) {
    std::string collection = sm.str(1);
    std::string variable_name = sm.str(2);
    std::transform(collection.begin(), collection.end(), collection.begin(), ::tolower);
    // TODO: support more collection
    if (collection == "tx") {
      macros.emplace_back(std::make_shared<Tx>(std::move(variable_name)));
    } else {
      assert(false);
    }
    macro_str = sm.suffix();
  }

  if (macros.empty()) {
    return nullptr;
  }

  if (macros.size() == 1) {
    return macros[0];
  }

  return std::make_shared<MultiMacro>(std::string(macro), std::move(macros));
}
} // namespace Macro
} // namespace SrSecurity