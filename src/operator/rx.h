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
#include <variant>

#include "operator_base.h"

#include "../common/assert.h"
#include "../common/literal_match/scanner.h"
#include "../common/pcre/scanner.h"
#include "../common/re2/scanner.h"
#include "../engine.h"
#include "../transaction.h"

namespace Wge {
namespace Operator {
/**
 * Performs a regular expression match of the pattern provided as parameter. This is the default
 * operator; the rules that do not explicitly specify an operator default to @rx.
 */
class Rx final : public OperatorBase {
  DECLARE_OPERATOR_NAME(rx);

public:
  Rx(std::string&& literal_value, bool is_not, std::string_view curr_rule_file_path)
      : OperatorBase(std::move(literal_value), is_not), scanner_(createScanner(literalValue())) {}

  Rx(std::unique_ptr<Macro::MacroBase>&& macro, bool is_not, std::string_view curr_rule_file_path)
      : OperatorBase(std::move(macro), is_not) {}

public:
  bool evaluate(Transaction& t, const Common::Variant& operand) const override {
    if (!IS_STRING_VIEW_VARIANT(operand))
      [[unlikely]] { return false; }

    const Scanner* scanner = &scanner_;

    // If there is a macro, expand it and create or reuse a scanner.
    if (macro_)
      [[unlikely]] {
        MACRO_EXPAND_STRING_VIEW(macro_value);

        // All the threads will try to access the macro_pcre_cache_ at the same time, so we need to
        // lock the macro_chche_mutex_.
        // May be we can use thread local storage to store the scanner, to avoid the lock. But the
        // probablity of the macro expansion is very low, so we use the lock here.
        std::lock_guard<std::mutex> lock(macro_chche_mutex_);

        auto iter = macro_scanner_cache_.find(macro_value);
        if (iter == macro_scanner_cache_.end()) {
          // To avoid copying the macro value when we find scanner in the macro_scanner_cache_ by
          // std::string type key, we use std::string_view type key to find scanner in the
          // macro_scanner_cache_, And store the macro value in the macro_value_cache_.
          macro_value_cache_.emplace_front(macro_value);
          auto macro_scanner = createScanner(macro_value);
          scanner =
              &(macro_scanner_cache_.emplace(macro_value_cache_.front(), std::move(macro_scanner))
                    .first->second);
        } else {
          scanner = &iter->second;
        }
      }

    std::vector<std::pair<size_t, size_t>> result;
    const std::string_view& operand_str = std::get<std::string_view>(operand);
    std::visit(
        [&](auto&& arg) {
          using T = std::decay_t<decltype(arg)>;
          if constexpr (std::is_same_v<T, std::unique_ptr<Common::Pcre::Scanner>>) {
            if (t.getEngine().config().pcre_match_limit_) {
              arg->setMatchLimit(t.getEngine().config().pcre_match_limit_);
            }
          }
          arg->match(operand_str, result);
        },
        *scanner);

    size_t capture_index = 0;
    for (const auto& [from, to] : result) {
      t.stageCapture(capture_index++, {operand_str.data() + from, to - from});
    }

    return !result.empty();
  }

public:
  /**
   * Set whether to capture the matched string.
   * @param capture true to capture, false not to capture.
   */
  void capture(bool capture) {
    ASSERT_IS_MAIN_THREAD();

    if (capture != capture_) {
      capture_ = capture;
      scanner_ = createScanner(literalValue());
    }
  }

  /**
   * Get whether to capture the matched string.
   * @return true to capture, false not to capture.
   */
  bool capture() const { return capture_; }

private:
  using Scanner =
      std::variant<std::unique_ptr<Common::Re2::Scanner>, std::unique_ptr<Common::Pcre::Scanner>,
                   std::unique_ptr<Common::LiteralMatch::Scanner>>;

private:
  Scanner createScanner(std::string_view pattern) const {
    Scanner scanner;
    if (Common::LiteralMatch::Scanner::isLiteralPattern(pattern)) {
      scanner = std::make_unique<Common::LiteralMatch::Scanner>(pattern, false);
    } else {
      auto re2 = std::make_unique<Common::Re2::Scanner>(pattern, false, capture_);
      if (re2->ok()) {
        scanner = std::move(re2);
      } else {
        WGE_LOG_WARN("Failed to compile RE2 pattern '{}': {}. Use PCRE instead.", pattern,
                     re2->error());
        scanner = std::make_unique<Common::Pcre::Scanner>(pattern, false, capture_);
      }
    }
    return scanner;
  }

private:
  Scanner scanner_;
  bool capture_{false};
  static std::forward_list<std::string> macro_value_cache_;
  static std::unordered_map<std::string_view, Scanner> macro_scanner_cache_;
  static std::mutex macro_chche_mutex_;
};
} // namespace Operator
} // namespace Wge