#include "set_rsc.h"

namespace SrSecurity {
namespace Action {
SetRsc::SetRsc(std::string&& value) : value_(std::move(value)) {}

SetRsc::SetRsc(std::shared_ptr<Macro::MacroBase> macro) : macro_(macro) {}

void SetRsc::evaluate(Transaction& t) const { throw "Not implemented!"; }
} // namespace Action
} // namespace SrSecurity