#include "factory.h"

#include <assert.h>

#include "highest_severity.h"
#include "matched_var.h"
#include "matched_var_name.h"
#include "multipart_strict_error.h"
#include "remote_addr.h"
#include "rule.h"
#include "session.h"
#include "tx.h"
#include "user_id.h"

namespace SrSecurity {
namespace Macro {
std::unordered_map<std::string,
                   std::function<std::unique_ptr<Macro::MacroBase>(std::string&& value)>>
    Factory::factory_ = {
        {"tx", [](std::string&& value) { return std::make_unique<Tx>(std::move(value)); }},
};

std::unique_ptr<Macro::MacroBase> Factory::instance(std::string&& value) {
  assert(value.starts_with("%{") && value.ends_with("}"));
  std::string key;
  auto pos = value.find('.');
  if (pos != std::string::npos) {
    key = value.substr(2, pos - 2);
  } else {
    key = value.substr(2);
  }

  auto iter = factory_.find(key);
  if (iter != factory_.end()) {
    return (iter->second)(std::move(value));
  }

  return nullptr;
}
} // namespace Macro
} // namespace SrSecurity