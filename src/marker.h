#pragma once

#include <array>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "rule.h"

namespace SrSecurity {
/**
 * Adds a fixed rule marker that can be used as a target in a skipAfter action. A SecMarker
 * directive essentially creates a rule that does nothing and whose only purpose is to carry the
 * given ID.
 */
class Marker {
public:
  constexpr static size_t phase_total_ = 5;
  Marker(std::string&& name, std::array<const Rule*, phase_total_>&& prev_rules);

public:
  /**
   * Get the name of the marker.
   * @return the name of the marker.
   */
  const std::string& name() const { return name_; }

  /**
   * Get the previous rule.
   * @param phase the phase of the previous rule.
   * @return the previous rule that in the given phase.
   */
  const Rule* prevRule(int phase) const { return prev_rules_[phase - 1]; }

  /**
   * Set the previous rule iterator.
   * @param prev_rule_iter the previous rule iterator.
   * @param phase specify the phase of the previous rule.
   */
  void prevRuleIter(std::vector<const Rule*>::iterator prev_rule_iter, int phase) {
    prev_rules_iter_[phase - 1] = prev_rule_iter;
  }

  /**
   * Get the previous rule iterator.
   * @param phase the phase of the previous rule.
   * @return the previous rule iterator that in the given phase.
   */
  const std::optional<std::vector<const Rule*>::iterator> prevRuleIter(int phase) const {
    return prev_rules_iter_[phase - 1];
  }

private:
  std::string name_;
  std::array<const Rule*, phase_total_> prev_rules_;
  std::array<std::optional<std::vector<const Rule*>::iterator>, phase_total_> prev_rules_iter_;
};
} // namespace SrSecurity