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
#include "macro_logic_matcher.h"

#include "../engine.h"
#include "../macro/variable_macro.h"
#include "../variable/ptree.h"

namespace Wge {
namespace Operator {
Operator::MacroLogicMatcher::MacroLogicMatcher(std::unique_ptr<Macro::MacroBase>&& macro)
    : macro_(std::move(macro)) {
  auto var_macro = dynamic_cast<Macro::VariableMacro*>(macro_.get());
  if (var_macro) {
    ptree_ = dynamic_cast<const Variable::PTree*>(var_macro->getVariable().get());
  }
}

bool MacroLogicMatcher::match(Transaction& t, const Common::Variant& left_operand, bool empty_match,
                              Matcher matcher, void* user_data) const {
  if (!ptree_) {
    return simpleMatch(t, left_operand, empty_match, matcher, user_data);
  } else {
    return complexMatch(t, left_operand, empty_match, matcher, user_data);
  }
}

bool MacroLogicMatcher::simpleMatch(Transaction& t, const Common::Variant& left_operand,
                                    bool empty_match, Matcher matcher, void* user_data) const {
  return std::visit(
      [&](auto&& value) -> bool {
        Common::EvaluateResults result;
        macro_->evaluate(t, result);
        if (result.empty() || IS_EMPTY_VARIANT(result.front().variant_)) {
          return empty_match;
        }

        if constexpr (std::is_same_v<std::decay_t<decltype(value)>, int64_t>) {
          if (IS_INT_VARIANT(result.front().variant_)) {
            return matcher(t, left_operand, result.front(), user_data);
          } else {
            return false;
          }
        } else if constexpr (std::is_same_v<std::decay_t<decltype(value)>, std::string_view>) {
          if (IS_STRING_VIEW_VARIANT(result.front().variant_)) {
            return matcher(t, left_operand, result.front(), user_data);
          } else {
            return false;
          }
        }

        return false;
      },
      left_operand);
}

bool MacroLogicMatcher::complexMatch(Transaction& t, const Common::Variant& left_operand,
                                     bool empty_match, Matcher matcher, void* user_data) const {
  WGE_LOG_TRACE("Start complex macro logic match...");
  return matchPtreeNode(t, left_operand, empty_match, matcher, user_data,
                        &t.getEngine().propertyTree(), 0);
}

bool matchWithAndOr(Variable::PTree::Path::Flag and_or, const Engine::PropertyTree& node,
                    Transaction& t, const Common::Variant& left_operand, bool empty_match,
                    MacroLogicMatcher::Matcher matcher, void* user_data) {
  if (node.empty()) {
    return empty_match;
  }

  if (and_or == Variable::PTree::Path::Flag::And) {
    for (const auto& [key, child_tree] : node) {
      if (!matcher(t, left_operand, {child_tree.data(), key}, user_data)) {
        return false;
      }
    }
    return true;
  } else if (and_or == Variable::PTree::Path::Flag::Or) {
    for (const auto& [key, child_tree] : node) {
      if (matcher(t, left_operand, {child_tree.data(), key}, user_data)) {
        return true;
      }
    }
    return false;
  } else {
    UNREACHABLE();
  }

  return false;
}

bool MacroLogicMatcher::matchPtreeNode(Transaction& t, const Common::Variant& left_operand,
                                       bool empty_match, Matcher matcher, void* user_data,
                                       const void* node, size_t path_index) const {
  const Engine::PropertyTree* current_node = static_cast<const Engine::PropertyTree*>(node);
  auto& paths = ptree_->paths();
  bool matched = false;
  for (size_t i = path_index; i < paths.size(); ++i) {
    WGE_LOG_TRACE([&]() {
      auto& path = paths[i];
      std::string space(i * 2, ' ');
      std::string flag;
      switch (path.type_) {
      case Variable::PTree::Path::Type::Map:
        if (i == paths.size() - 1) {
          if (path.flag_ == Variable::PTree::Path::Flag::And) {
            flag = "{&}";
          } else if (path.flag_ == Variable::PTree::Path::Flag::Or) {
            flag = "{}";
          }
        }
        break;
      case Variable::PTree::Path::Type::Array:
        if (path.flag_ == Variable::PTree::Path::Flag::And) {
          flag = "[&]";
        } else if (path.flag_ == Variable::PTree::Path::Flag::Or) {
          flag = "[]";
        }
        break;
      }

      std::string dot = (i == 0) ? "" : ".";
      return std::format("{}{}{}{}", space, dot, paths[i].name_, flag);
    }());

    auto& path = paths[i];
    auto child = current_node->get_child_optional(path.name_);
    if (!child) {
      WGE_LOG_WARN("The node '{}' is not found in the property tree. Node index: {}", path.name_,
                   i);
      matched = empty_match;
      break;
    }
    current_node = &child.get();
    switch (path.type_) {
    case Variable::PTree::Path::Type::Map: {
      // If it's the last node and it's a map, we match the values of the map
      if (i == paths.size() - 1) {
        matched = matchWithAndOr(path.flag_, *current_node, t, left_operand, empty_match, matcher,
                                 user_data);
      }
    } break;
    case Variable::PTree::Path::Type::Array: {
      // If it's the last node and it's an array, we match the values of the array
      if (i == paths.size() - 1) {
        matched = matchWithAndOr(path.flag_, *current_node, t, left_operand, empty_match, matcher,
                                 user_data);
      } else {
        // Otherwise, we walk through each element in the array
        switch (path.flag_) {
        case Variable::PTree::Path::Flag::And:
          for (const auto& [key, child_tree] : *current_node) {
            if (!matchPtreeNode(t, left_operand, empty_match, matcher, user_data, &child_tree,
                                i + 1)) {
              return false;
            }
          }
          return true;
        case Variable::PTree::Path::Flag::Or:
          for (const auto& [key, child_tree] : *current_node) {
            if (matchPtreeNode(t, left_operand, empty_match, matcher, user_data, &child_tree,
                               i + 1)) {
              return true;
            }
          }
          return false;
        default:
          UNREACHABLE();
          return false;
        }
      }
    } break;
    case Variable::PTree::Path::Type::Value: {
      auto value = current_node->data();
      assert(!IS_EMPTY_VARIANT(value));
      matched = matcher(t, left_operand, {value, path.name_}, user_data);
    } break;
    default:
      break;
    }
  }

  return matched;
}
} // namespace Operator
} // namespace Wge