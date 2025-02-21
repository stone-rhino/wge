#include "set_uid.h"

namespace SrSecurity {
namespace Action {
SetUid::SetUid(std::string&& value) : value_(std::move(value)) {}

SetUid::SetUid(std::shared_ptr<Macro::MacroBase> macro) : macro_(macro) {}

void SetUid::evaluate(Transaction& t) const { throw "Not implemented!"; }
} // namespace Action
} // namespace SrSecurity