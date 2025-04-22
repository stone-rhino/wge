/**
 * Copyright (c) 2024-2025 Stone Rhino and contributors.
 *
 * MIT License (http://opensource.org/licenses/MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
 * associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
 * NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#pragma once

#include <forward_list>
#include <memory>
#include <mutex>
#include <unordered_map>

#include "operator_base.h"

#include "../common/assert.h"
#include "../common/pcre/scanner.h"
#include "../engine.h"
#include "../transaction.h"

namespace Wge {
namespace Operator {
/**
 * Performs a regular expression match of the pattern provided as parameter. This is the default
 * operator; the rules that do not explicitly specify an operator default to @rx.
 */
class Rx : public OperatorBase {
  DECLARE_OPERATOR_NAME(rx);

public:
  Rx(std::string&& literal_value, bool is_not, std::string_view curr_rule_file_path)
      : OperatorBase(std::move(literal_value), is_not),
        pcre_(std::make_unique<Common::Pcre::Scanner>(literalValue(), false, capture_)) {}

  Rx(const std::shared_ptr<Macro::MacroBase> macro, bool is_not,
     std::string_view curr_rule_file_path)
      : OperatorBase(macro, is_not) {}

public:
  bool evaluate(Transaction& t, const Common::Variant& operand) const override {
    if (!IS_STRING_VIEW_VARIANT(operand)) [[unlikely]] {
      return false;
    }

    Common::Pcre::Scanner* scanner = pcre_.get();

    // If there is a macro, expand it and create or reuse a scanner.
    if (macro_) [[unlikely]] {
      MACRO_EXPAND_STRING_VIEW(macro_value);

      // All the threads will try to access the macro_pcre_cache_ at the same time, so we need to
      // lock the macro_chche_mutex_.
      // May be we can use thread local storage to store the scanner, to avoid the lock. But the
      // probablity of the macro expansion is very low, so we use the lock here.
      std::lock_guard<std::mutex> lock(macro_chche_mutex_);

      auto iter = macro_pcre_cache_.find(macro_value);
      if (iter == macro_pcre_cache_.end()) {
        // To avoid copying the macro value when we find scanner in the macro_pcre_cache_ by
        // std::string type key, we use std::string_view type key to find scanner in the
        // macro_pcre_cache_, And store the macro value in the macro_value_cache_.
        macro_value_cache_.emplace_front(macro_value);
        auto macro_scanner = std::make_unique<Common::Pcre::Scanner>(macro_value, false, capture_);
        scanner = macro_scanner.get();
        macro_pcre_cache_.emplace(macro_value_cache_.front(), std::move(macro_scanner));
      } else {
        scanner = iter->second.get();
      }
    }

    if (t.getEngine().config().pcre_match_limit_) {
      scanner->setMatchLimit(t.getEngine().config().pcre_match_limit_);
    }

    // Match the operand with the pattern.
    std::vector<std::pair<size_t, size_t>> result;
    const std::string_view& operand_str = std::get<std::string_view>(operand);
    scanner->match(operand_str, result);

    for (const auto& [from, to] : result) {
      Common::EvaluateResults::Element value;
      value.variant_ = std::string_view(operand_str.data() + from, to - from);
      t.addCapture(std::move(value));
    }

    return is_not_ ^ (!result.empty());
  }

public:
  /**
   * Set whether to capture the matched string.
   * @param capture true to capture, false not to capture.
   */
  void capture(bool capture) {
    ASSERT_IS_MAIN_THREAD();

    if (capture != capture_) {
      pcre_ = std::make_unique<Common::Pcre::Scanner>(literalValue(), false, capture);
      capture_ = capture;
    }
  }

  /**
   * Get whether to capture the matched string.
   * @return true to capture, false not to capture.
   */
  bool capture() const { return capture_; }

private:
  std::unique_ptr<Common::Pcre::Scanner> pcre_;
  bool capture_{false};
  static std::forward_list<std::string> macro_value_cache_;
  static std::unordered_map<std::string_view, std::unique_ptr<Common::Pcre::Scanner>>
      macro_pcre_cache_;
  static std::mutex macro_chche_mutex_;
};
} // namespace Operator
} // namespace Wge