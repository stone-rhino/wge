#pragma once

#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "rule.h"

namespace SrSecurity {
class Marker {
public:
  Marker(std::string&& name, Rule* prev_rule);

public:
  void init(std::vector<Rule*>::iterator prev_rule_iter) { prev_rule_iter_ = prev_rule_iter; }
  const std::string& name() const { return name_; }
  Rule* prevRule() const { return prev_rule_; }
  const std::optional<std::vector<Rule*>::iterator>& prevRuleIter() const {
    return prev_rule_iter_;
  }

private:
  std::string name_;
  Rule* prev_rule_;
  std::optional<std::vector<Rule*>::iterator> prev_rule_iter_;
};
} // namespace SrSecurity