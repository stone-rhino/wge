#pragma once

#include "variable_base.h"

namespace SrSecurity {
namespace Variable {
/**
 * This variable holds an identifier intended to be unique to the each transaction. The ModSecurity
 * v3 implementation is to use a millisecond timestamp, followed by a dot character ('.'), followed
 * by a random six-digit number.
 */
class UniqueId : public VariableBase {
  DECLARE_VIRABLE_NAME(UNIQUE_ID);

public:
  UniqueId(std::string&& sub_name, bool is_not, bool is_counter)
      : VariableBase(std::move(sub_name), is_not, is_counter) {}

public:
  std::string_view evaluate(Transaction& t) const override { return t.getUniqueId(); };
};
} // namespace Variable
} // namespace SrSecurity