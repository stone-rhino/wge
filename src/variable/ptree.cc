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
void PTree::initPaths(const std::string& sub_name, std::vector<Path>& paths) {
  std::vector<std::string_view> tokens = Common::SplitTokens(sub_name, '.');
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

    paths.emplace_back(std::move(path));
  }
}

void PTree::evaluateNode(const Common::PropertyTree* node, const std::vector<Path>& paths,
                         size_t path_index, Common::EvaluateResults& result) {
  const Common::PropertyTree* current_node = node;
  for (size_t i = path_index; i < paths.size(); ++i) {
    auto& path = paths[i];
    switch (path.type_) {
    case Path::Type::Map: {
      auto child = current_node->get_child_optional(path.name_);
      if (!child) {
        WGE_LOG_WARN("The map node '{}' is not found in the property tree.", path.name_);
        result.clear();
        return;
      }
      current_node = static_cast<const Common::PropertyTree*>(&child.get());

      // If it's the last node and it's a map, we return the values of the map
      if (i == paths.size() - 1) {
        for (const auto& [key, child_tree] : *current_node) {
          result.emplace_back(child_tree.data(), key,
                              static_cast<const Common::PropertyTree*>(&child_tree));
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
      current_node = static_cast<const Common::PropertyTree*>(&child.get());

      if (i == paths.size() - 1) {
        // If it's the last node and it's an array, we return the values of the array
        for (const auto& [key, child_tree] : *current_node) {
          result.emplace_back(child_tree.data(), key,
                              static_cast<const Common::PropertyTree*>(&child_tree));
        }
      } else {
        // Otherwise, we walk through each element in the array
        ++i;
        for (const auto& [key, child_tree] : *current_node) {
          evaluateNode(static_cast<const Common::PropertyTree*>(&child_tree), paths, i, result);
        }
      }
    } break;
    case Path::Type::Value: {
      auto child = current_node->get_child_optional(path.name_);
      if (!child) {
        WGE_LOG_WARN("The value node '{}' is not found in the property tree.", path.name_);
        result.clear();
        return;
      }
      result.emplace_back(child->data(), path.name_,
                          static_cast<const Common::PropertyTree*>(&child.get()));
    } break;
    default:
      break;
    }
  }
}

void PTree::evaluateNode(const Common::PropertyTree* node, Common::EvaluateResults& result) {
  if (node->empty()) {
    result.emplace_back(node->data(), "", static_cast<const Common::PropertyTree*>(node));
  } else {
    for (const auto& [_, child_tree] : *node) {
      if (child_tree.empty()) {
        result.emplace_back(child_tree.data(), "",
                            static_cast<const Common::PropertyTree*>(&child_tree));
      } else {
        evaluateNode(static_cast<const Common::PropertyTree*>(&child_tree), result);
      }
    }
  }
}
} // namespace Variable
} // namespace Wge