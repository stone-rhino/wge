#pragma once

#include "variable_base.h"

namespace SrSecurity {
namespace Variable {
class RequestUriRaw : public VariableBase {
  DECLARE_VIRABLE_NAME(REQUEST_URI_RAW);

public:
  RequestUriRaw(std::string&& sub_name, bool is_not, bool is_counter)
      : VariableBase(std::move(sub_name), is_not, is_counter) {}

public:
  const Common::Variant& evaluate(Transaction& t) const override { assert(false); throw "Not implemented!"; };
};
} // namespace Variable
} // namespace SrSecurity