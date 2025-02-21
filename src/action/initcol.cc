#include "initcol.h"

namespace SrSecurity {
namespace Action {
InitCol::InitCol(std::string&& key, std::string&& value)
    : key_(std::move(key)), value_(std::move(value)) {}

void InitCol::evaluate(Transaction& t) const {}
} // namespace Action
} // namespace SrSecurity