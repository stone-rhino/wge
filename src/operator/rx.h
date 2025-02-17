#pragma once

#include <assert.h>

#include "operator_base.h"

#include "../common/pcre.h"
#include "../transaction.h"

namespace SrSecurity {
namespace Operator {
/**
 * Performs a regular expression match of the pattern provided as parameter. This is the default
 * operator; the rules that do not explicitly specify an operator default to @rx.
 */
class Rx : public OperatorBase {
  DECLARE_OPERATOR_NAME(rx);

public:
  Rx(std::string&& literal_value)
      : OperatorBase(std::move(literal_value)), pcre_(literalValue(), false) {}

public:
  bool evaluate(Transaction& t, std::string_view operand) const override {
    // Match the operand with the pattern.
    auto result = pcre_.match(operand, per_thread_pcre_scratch_);

    // Ignore capture_ and set the match result directly, because we need to capture the
    // matched string for %{MATCHED_VAR} in the rule action.
    size_t index = 0;
    for (const auto& [from, to] : result) {
      t.setMatched(index++, std::string_view(operand.data() + from, to - from));
    }

    return !result.empty();
  }

public:
  /**
   * Set whether to capture the matched string.
   * @param capture true to capture, false not to capture.
   */
  void setCapture(bool capture) { capture_ = capture; }

  /**
   * Get whether to capture the matched string.
   * @return true to capture, false not to capture.
   */
  bool getCapture() const { return capture_; }

private:
  Common::Pcre pcre_;
  bool capture_{false};
  static thread_local Common::Pcre::Scratch per_thread_pcre_scratch_;
};
} // namespace Operator
} // namespace SrSecurity