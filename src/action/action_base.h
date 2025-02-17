#pragma once

#include <string>

#include "../transaction.h"

namespace SrSecurity {
namespace Action {
class ActionBase {
public:
  virtual void evaluate(Transaction& t) const = 0;
};
} // namespace Action
} // namespace SrSecurity