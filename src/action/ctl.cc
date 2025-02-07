#include "ctl.h"

#include <stdlib.h>

namespace SrSecurity {
namespace Action {
Ctl::Ctl(CtlType type, std::any&& value) : type_(type), value_(std::move(value)) {}

void Ctl::evaluate(Transaction& t) {}
} // namespace Action
} // namespace SrSecurity