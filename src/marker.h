#pragma once

#include <memory>
#include <string>
#include <vector>

#include "rule.h"

namespace SrSecurity {
class Marker {
public:
  Marker(std::string&& name, Rule* prev_rule);

private:
  std::string name_;
  Rule* prev_rule_;
};
} // namespace SrSecurity