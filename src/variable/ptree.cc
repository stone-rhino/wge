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
#include "ptree.h"

#include "../common/log.h"
#include "../common/string.h"
#include "../engine.h"

namespace Wge {
namespace Variable {
void Variable::PTree::evaluate(Transaction& t, Common::EvaluateResults& result) const {
  evaluateNode(&t.getEngine().propertyTree(), 0, result);
}

void PTree::initPaths() {
  std::vector<std::string_view> tokens = Common::SplitTokens(sub_name_, '.');
  for (size_t i = 0; i < tokens.size(); ++i) {
    auto& token = tokens[i];
    Path path;
    if (token.ends_with("]")) {
      if (token[token.size() - 2] == '&') {
        path.name_ = std::string(token.substr(0, token.size() - 3));
        path.flag_ = Path::Flag::And;
      } else {
        path.name_ = std::string(token.substr(0, token.size() - 2));
        path.flag_ = Path::Flag::Or;
      }
      path.type_ = Path::Type::Array;
    } else if (token.ends_with("}")) {
      if (token[token.size() - 2] == '&') {
        path.name_ = std::string(token.substr(0, token.size() - 3));
        path.flag_ = Path::Flag::And;
      } else {
        path.name_ = std::string(token.substr(0, token.size() - 2));
        path.flag_ = Path::Flag::Or;
      }
      path.type_ = Path::Type::Map;
    } else {
      path.name_ = std::string(token);
      path.type_ = i == tokens.size() - 1 ? Path::Type::Value : Path::Type::Map;
      path.flag_ = Path::Flag::Single;
    }

    paths_.emplace_back(std::move(path));
  }
}

void PTree::evaluateNode(const void* node, size_t path_index,
                         Common::EvaluateResults& result) const {
  const Engine::PropertyTree* current_node = static_cast<const Engine::PropertyTree*>(node);
  for (size_t i = path_index; i < paths_.size(); ++i) {
    auto& path = paths_[i];
    switch (path.type_) {
    case Path::Type::Map: {
      auto child = current_node->get_child_optional(path.name_);
      if (!child) {
        WGE_LOG_WARN("The map node '{}' is not found in the property tree.", path.name_);
        result.clear();
        return;
      }
      current_node = &child.get();

      // If it's the last node and it's a map, we return the values of the map
      if (i == paths_.size() - 1) {
        for (const auto& [key, child_tree] : *current_node) {
          result.emplace_back(child_tree.data(), key);
        }
      }
    } break;
    case Path::Type::Array: {
      auto child = current_node->get_child_optional(path.name_);
      if (!child) {
        WGE_LOG_WARN("The array node '{}' is not found in the property tree.", path.name_);
        result.clear();
        return;
      }
      current_node = &child.get();

      if (i == paths_.size() - 1) {
        // If it's the last node and it's an array, we return the values of the array
        for (const auto& [key, child_tree] : *current_node) {
          result.emplace_back(child_tree.data(), key);
        }
      } else {
        // Otherwise, we walk through each element in the array
        ++i;
        for (const auto& [key, child_tree] : *current_node) {
          evaluateNode(&child_tree, i, result);
        }
      }
    } break;
    case Path::Type::Value: {
      auto value = current_node->get_optional<Common::Variant>(path.name_);
      if (!value) {
        WGE_LOG_WARN("The value node '{}' is not found in the property tree.", path.name_);
        result.clear();
        return;
      }
      result.emplace_back(value.get(), path.name_);
    } break;
    default:
      break;
    }
  }
}
} // namespace Variable
} // namespace Wge