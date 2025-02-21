#pragma once

#include <string>

#include "../transaction.h"

#define DECLARE_ACTION_NAME(n)                                                                     \
public:                                                                                            \
  const char* name() const override { return name_; }                                              \
                                                                                                   \
private:                                                                                           \
  static constexpr char name_[] = #n;

namespace SrSecurity {
namespace Action {
class ActionBase {
public:
  virtual ~ActionBase() = default;

public:
  virtual void evaluate(Transaction& t) const = 0;
  virtual const char* name() const = 0;
};
} // namespace Action
} // namespace SrSecurity