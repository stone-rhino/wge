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

#include <boost/property_tree/ptree.hpp>

#include "variant.h"

namespace Wge {
namespace Common {
class PropertyTree;

struct PropertyTreeValue : public Common::Variant {
  PropertyTree* parent_{nullptr};
};

/**
 * PropertyTree is a subclass of boost::property_tree::basic_ptree with PropertyTreeValue as the
 * data type. It adds a parent pointer to each node for accessing parent node information when
 * needed. Its value type is PropertyTreeValue, which inherits from Common::Variant and can store
 * multiple data types.
 */
class PropertyTree : public boost::property_tree::basic_ptree<std::string, PropertyTreeValue> {
public:
  PropertyTree(PropertyTree* parent = nullptr)
      : boost::property_tree::basic_ptree<std::string, PropertyTreeValue>() {
    this->data().parent_ = parent;
  }

public:
  PropertyTree* parent() const { return static_cast<PropertyTree*>(this->data().parent_); }
  std::string dump() const {
    std::string output;
    dumpInner(this, output, "", 0);

    // Remove the last newline character
    if (!output.empty() && output.back() == '\n') {
      output.pop_back();
    }

    return output;
  }

private:
  static void dumpInner(const Common::PropertyTree* node, std::string& output,
                        const std::string& node_name, size_t indent) {
    const Common::PropertyTree* current_node = node;
    bool is_array = false;
    if (!current_node->empty()) {
      is_array = std::all_of(current_node->begin(), current_node->end(), [](const auto& pair) {
        if (pair.first.empty()) {
          return true;
        }
        return false;
      });
    }

    if (is_array) {
      if (node_name.empty()) {
        output += std::string(indent, ' ') + "[\n";
      } else {
        output += std::string(indent, ' ') + node_name + ": [\n";
      }
      for (const auto& [key, child_tree] : *current_node) {
        if (child_tree.empty()) {
          output += std::format("{}{}\n", std::string(indent + 2, ' '),
                                VISTIT_VARIANT_AS_STRING(child_tree.data()));
        } else {
          dumpInner(static_cast<const Common::PropertyTree*>(&child_tree), output, key, indent + 2);
        }
      }
      output += std::string(indent, ' ') + "]\n";
    } else {
      bool is_map = !current_node->empty();
      if (is_map) {
        if (node_name.empty()) {
          output += std::string(indent, ' ') + "{\n";
        } else {
          output += std::string(indent, ' ') + node_name + ": {\n";
        }
        for (const auto& [key, child_tree] : *current_node) {
          if (child_tree.empty()) {
            output += std::format("{}{}: {}\n", std::string(indent + 2, ' '), key,
                                  VISTIT_VARIANT_AS_STRING(child_tree.data()));
          } else {
            dumpInner(static_cast<const Common::PropertyTree*>(&child_tree), output, key,
                      indent + 2);
          }
        }
        output += std::string(indent, ' ') + "}\n";
      } else {
        if (node_name.empty()) {
          output += std::format("{}{}\n", std::string(indent, ' '),
                                VISTIT_VARIANT_AS_STRING(current_node->data()));
        } else {
          output += std::format("{}{}: {}\n", std::string(indent, ' '), node_name,
                                VISTIT_VARIANT_AS_STRING(current_node->data()));
        }
      }
    }
  }
};

} // namespace Common
} // namespace Wge