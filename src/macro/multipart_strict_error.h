#pragma once

#include "macro_base.h"

#include "../config.h"

namespace SrSecurity {
namespace Macro {
class MultipartStrictError : public MacroBase {
public:
  MultipartStrictError(SrSecurity::MultipartStrictError::ErrorType type) { type_ = type; }

public:
  const Common::Variant& evaluate(Transaction& t) override {
    assert(false);
    throw "Not implemented!";
  }

private:
  SrSecurity::MultipartStrictError::ErrorType type_;
};
} // namespace Macro
} // namespace SrSecurity