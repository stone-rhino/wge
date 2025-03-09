#pragma once

#include <memory>

#include "operator_base.h"

#include "../common/assert.h"
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
  Rx(std::string&& literal_value, bool is_not)
      : OperatorBase(std::move(literal_value), is_not),
        pcre_(std::make_unique<Common::Pcre>(literalValue(), false)) {}

  Rx(const std::shared_ptr<Macro::MacroBase> macro, bool is_not) : OperatorBase(macro, is_not) {}

public:
  bool evaluate(Transaction& t, const Common::Variant& operand) const override {
    if (IS_EMPTY_VARIANT(operand)) [[unlikely]] {
      return false;
    }

    if (!pcre_) [[unlikely]] {
      // TODO(zhouyu 20250305): Initialize the pcre_ object with macro evaluated value. And use a
      // hash table to store the pcre_ object to avoid recompiling the same pattern.
      return false;
    }

    std::vector<std::pair<size_t, size_t>> result;

    // Match the operand with the pattern.
    if (IS_STRING_VIEW_VARIANT(operand)) [[likely]] {
      const std::string_view& operand_str = std::get<std::string_view>(operand);
      result = pcre_->match(operand_str, per_thread_pcre_scratch_);

      // Ignore capture_ and set the match result directly, because we need to capture the
      // matched string for %{MATCHED_VAR} in the rule action.
      size_t index = 0;
      for (const auto& [from, to] : result) {
        t.setMatched(index++, std::string_view(operand_str.data() + from, to - from));
      }
    } else {
      UNREACHABLE();
    }

    return is_not_ ? result.empty() : !result.empty();
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
  std::unique_ptr<Common::Pcre> pcre_;
  bool capture_{false};

  // The result of the regular expression match.
  // All threads share the same rule object, that means all threads share the same operator object.
  // So we need to use thread_local to avoid.
  static thread_local Common::Pcre::Scratch per_thread_pcre_scratch_;
};
} // namespace Operator
} // namespace SrSecurity