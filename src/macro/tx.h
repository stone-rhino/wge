#pragma once

#include "macro_base.h"

#include "../common/log.h"

namespace SrSecurity {
namespace Macro {
class Tx : public MacroBase {
public:
  Tx(std::string&& variable_name) : variable_name_(std::move(variable_name)) {
    if (std::all_of(variable_name_.begin(), variable_name_.end(), ::isdigit)) {
      matched_index_ = ::atoi(variable_name_.c_str());
    }
  }

public:
  const Common::Variant& evaluate(Transaction& t) override {
    Common::Variant* buffer = &t.evaluatedBuffer().macro_;
    if (matched_index_ == 0xffffffff) {
      SRSECURITY_LOG_TRACE("macro %{{TX.{}}} expanded: {}", variable_name_,
                           VISTIT_VARIANT_AS_STRING(t.getVariable(variable_name_)));
      // Avoiding copy of the variant, return a reference to the variant
      buffer = &const_cast<Common::Variant&>(t.getVariable(variable_name_));
    } else {
      SRSECURITY_LOG_TRACE("macro %{{TX.{}}} expanded: {}", variable_name_,
                           *t.getMatched(matched_index_));
      // Although the variant is copied, it is a string_view, so it is cheap to copy
      *buffer = *t.getMatched(matched_index_);
    }

    return *buffer;
  }

private:
  std::string variable_name_;
  size_t matched_index_{0xffffffff};
};
} // namespace Macro
} // namespace SrSecurity