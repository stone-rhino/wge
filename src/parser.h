#pragma once
#include <array>
#include <list>
#include <memory>
#include <string>
#include <unordered_set>

#include "rule/rule_base.h"

namespace SrSecurity {
class Parser {
public:
  void loadFromFile(const std::string& file_path);

public:
  const std::list<Rule::RuleSharedPtr>& getValidRules(size_t phase) const;

private:
  void fillValideRules(const std::unordered_set<uint64_t>& remove_ids,
                       const std::unordered_set<std::string>& remove_tags);

private:
  constexpr static size_t phase_total_ = 5;
  std::array<std::list<Rule::RuleSharedPtr>, phase_total_> all_rules_;
  std::array<std::list<Rule::RuleSharedPtr>, phase_total_> valid_rules_;
};
} // namespace SrSecurity
