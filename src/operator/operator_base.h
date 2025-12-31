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

#include <string>

#include <absl/container/inlined_vector.h>

#include "../common/log.h"
#include "../common/property_tree.h"
#include "../common/variant.h"
#include "../macro/macro_base.h"
#include "../transaction.h"

#define DECLARE_OPERATOR_NAME(n)                                                                   \
public:                                                                                            \
  const char* name() const override { return name_; }                                              \
                                                                                                   \
private:                                                                                           \
  static constexpr char name_[] = #n;

namespace Wge {
namespace Operator {
/**
 * Base class for all operators.
 */
class OperatorBase {
public:
  struct Result {
    bool matched_{false};
    std::string_view capture_;
    const Common::PropertyTree* ptree_node_{nullptr};
  };
  using Results = absl::InlinedVector<Result, 1>;

public:
  OperatorBase(std::string&& literal_value, bool is_not)
      : literal_value_(std::move(literal_value)), is_not_(is_not) {}

  OperatorBase(std::unique_ptr<Macro::MacroBase>&& macro, bool is_not)
      : macro_(std::move(macro)), is_not_(is_not) {}

  virtual ~OperatorBase() = default;

public:
  /**
   * Get the literal value of the operator.
   * @return the literal value of the operator.
   */
  const std::string& literalValue() const { return literal_value_; }

  /**
   * Get the macro of the operator.
   * @return the macro of the operator.
   */
  std::unique_ptr<Macro::MacroBase>& macro() { return macro_; }

  /**
   * Check if the operator is a NOT operator.
   * @return true if the operator is a NOT operator, false otherwise.
   */
  bool isNot() const { return is_not_; }

  /**
   * If enabled, and value of operator is a macro that evaluates to empty, the rule will match.
   * @param value true to enable empty match, false to disable.
   */
  void emptyMatch(bool value) { empty_match_ = value; }

  /**
   * Check if empty match is enabled.
   * @return true if empty match is enabled, false otherwise.
   */
  bool emptyMatch() const { return empty_match_; }

public:
  /**
   * Evaluate the operator.
   * @param t the transaction.
   * @param operand the operand to evaluate.
   * @param results the results of the evaluation.
   */
  virtual void evaluate(Transaction& t, const Common::Variant& operand, Results& results) const = 0;

  /**
   * Get the name of the operator.
   * @return the name of the operator.
   */
  virtual const char* name() const = 0;

protected:
  template <class LeftOperandT, class RightOperandT>
  void performComparison(Transaction& t, const Common::Variant& left_operand,
                         const RightOperandT& right_operand, Results& results,
                         void (*comparison)(Transaction& t, LeftOperandT left_operand,
                                            RightOperandT right_operand, Results& results,
                                            void* user_data),
                         void* user_data = nullptr) const {
    if (!macro_)
      [[likely]] {
        if (std::holds_alternative<LeftOperandT>(left_operand))
          [[likely]] {
            comparison(t, std::get<LeftOperandT>(left_operand), right_operand, results, user_data);
          }
        else {
          results.emplace_back(false);
        }
      }
    else {
      Common::EvaluateResults macro_result;
      macro_->evaluate(t, macro_result);
      if (macro_result.empty()) {
        results.emplace_back(empty_match_);
        return;
      }

      for (const auto& right_operand : macro_result) {
        if (std::holds_alternative<RightOperandT>(right_operand.variant_))
          [[likely]] {
            if (!std::holds_alternative<LeftOperandT>(left_operand))
              [[unlikely]] {
                results.emplace_back(false);
                continue;
              }

            size_t old_size = results.size();
            comparison(t, std::get<LeftOperandT>(left_operand),
                       std::get<RightOperandT>(right_operand.variant_), results, user_data);
            for (size_t i = old_size; i < results.size(); ++i) {
              results[i].ptree_node_ = right_operand.ptree_node_;
            }

            WGE_LOG_TRACE([&]() {
              std::string sub_name;
              if (!right_operand.variable_sub_name_.empty()) {
                sub_name = std::format("\"{}\":", right_operand.variable_sub_name_);
              }
              return std::format("{} @{} {}{} => {}", std::get<LeftOperandT>(left_operand), name(),
                                 sub_name, std::get<RightOperandT>(right_operand.variant_),
                                 results.back().matched_);
            }());
          }
        else if (IS_EMPTY_VARIANT(right_operand.variant_)) {
          results.emplace_back(empty_match_);
        } else {
          results.emplace_back(false);
        }
      }
    }
  }

protected:
  std::string literal_value_;
  bool is_not_;
  std::unique_ptr<Macro::MacroBase> macro_;
  bool empty_match_{false};
};
} // namespace Operator
} // namespace Wge