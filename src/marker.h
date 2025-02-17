#pragma once

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
  Marker(std::string&& name, const Rule* prev_rule);

public:
  /**
   * Initialize the marker with the previous rule iterator.
   * @param prev_rule_iter the previous rule iterator.
   */
  void init(std::vector<const Rule*>::iterator prev_rule_iter) { prev_rule_iter_ = prev_rule_iter; }

  /**
   * Get the name of the marker.
   * @return the name of the marker.
   */
  const std::string& name() const { return name_; }

  /**
   * Get the previous rule.
   * @return the previous rule.
   */
  const Rule* prevRule() const { return prev_rule_; }

  /**
   * Get the previous rule iterator.
   * @return the previous rule iterator.
   */
  const std::optional<std::vector<const Rule*>::iterator>& prevRuleIter() const {
    return prev_rule_iter_;
  }

private:
  std::string name_;
  const Rule* prev_rule_;
  std::optional<std::vector<const Rule*>::iterator> prev_rule_iter_;
};
} // namespace SrSecurity