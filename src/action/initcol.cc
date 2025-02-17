#include "initcol.h"

namespace SrSecurity {
namespace Action {
InitCol::InitCol(std::string&& name, std::string&& value)
    : name_(std::move(name)), value_(std::move(value)) {}

void InitCol::evaluate(Transaction& t) const {}
} // namespace Action
} // namespace SrSecurity