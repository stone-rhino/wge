#pragma once

#include <functional>
#include <memory>
#include <unordered_map>

#include "macro_base.h"

namespace SrSecurity {
namespace Macro {
class Factory {
public:
  static std::unique_ptr<Macro::MacroBase> instance(std::string&& value);

private:
  static std::unordered_map<std::string,
                            std::function<std::unique_ptr<Macro::MacroBase>(std::string&&)>>
      factory_;
};
} // namespace Macro
} // namespace SrSecurity