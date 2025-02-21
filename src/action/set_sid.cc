#include "set_sid.h"

namespace SrSecurity {
namespace Action {
SetSid::SetSid(std::string&& value) : value_(std::move(value)) {}

SetSid::SetSid(std::shared_ptr<Macro::MacroBase> macro) : macro_(macro) {}

void SetSid::evaluate(Transaction& t) const { throw "Not implemented!"; }
} // namespace Action
} // namespace SrSecurity