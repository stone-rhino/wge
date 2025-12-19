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

#include <memory>
#include <vector>

#include "../common/evaluate_result.h"
#include "../common/property_tree.h"
#include "../macro/macro_base.h"
#include "../transaction.h"

namespace Wge {
namespace Variable {
class PTree;
} // namespace Variable
} // namespace Wge

namespace Wge {
namespace Operator {
/**
 * Macro logic matcher for macro expansion with AND/OR logic.
 * If the macro is a PTree variable, the matcher will use the PTree paths to match the nodes.
 * The paths can have different match strategies: SingleMatch, AllMatch, AnyMatch.
 * Example: PTREE.world.country[&].province[].city[&].mayor.health{}
 * - []: Indicates that the node is a array, and any element match is sufficient (OR logic)
 * - {}: Indicates that the node is a map, and any value match is sufficient (OR logic)
 * - [&]: Indicates that the node is a array, and all elements must match (AND logic)
 * - {&}: Indicates that the node is a map, and all values must match (AND logic)
 */
class MacroLogicMatcher {
public:
  struct Node {
    enum class MatchStrategy {
      // Only match first element of macro expansion
      SingleMatch,
      // All elements of macro expansion must match (AND logic)
      AllMatch,
      // Any element of macro expansion can match (OR logic)
      AnyMatch
    };
    MatchStrategy strategy_;
    size_t structured_index_;
  };

public:
  MacroLogicMatcher(std::unique_ptr<Macro::MacroBase>&& macro);

public:
  using Matcher = bool (*)(Transaction& t, const Common::Variant& left_operand,
                           const Common::EvaluateElement& right_operand, void* user_data);

  /**
   * Evaluate the complex logic match against a match function.
   * @param t The transaction.
   * @param left_operand The left operand.
   * @param empty_match The result to return when there is no value of the macro expansion.
   * @param matcher The match function.
   * @return true if the match is successful, false otherwise.
   */
  bool match(Transaction& t, const Common::Variant& left_operand, bool empty_match, Matcher matcher,
             void* user_data) const;

  /**
   * Get the macro of the matcher.
   * @return the macro of the matcher.
   */
  std::unique_ptr<Macro::MacroBase>& macro() { return macro_; }

private:
  bool simpleMatch(Transaction& t, const Common::Variant& left_operand, bool empty_match,
                   Matcher matcher, void* user_data) const;
  bool complexMatch(Transaction& t, const Common::Variant& left_operand, bool empty_match,
                    Matcher matcher, void* user_data) const;
  bool matchPtreeNode(Transaction& t, const Common::Variant& left_operand, bool empty_match,
                      Matcher matcher, void* user_data, const Common::PropertyTree* node,
                      size_t path_index) const;

private:
  std::unique_ptr<Macro::MacroBase> macro_;
  const Variable::PTree* ptree_{nullptr};
};
} // namespace Operator
} // namespace Wge