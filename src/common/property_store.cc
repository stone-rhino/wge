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
#include "property_store.h"

#include <sstream>

#include <boost/property_tree/json_parser.hpp>

#include "log.h"

namespace Wge {
namespace Common {
std::expected<bool, std::string> PropertyStore::loadFromJsonString(const std::string& json_string) {
  std::istringstream iss(json_string);
  boost::property_tree::ptree temp_ptree;
  try {
    boost::property_tree::read_json(iss, temp_ptree);
  } catch (const boost::property_tree::json_parser_error& e) {
    WGE_LOG_ERROR("Failed to parse property tree JSON string: {}", e.what());
    return std::unexpected<std::string>(e.what());
  }

  string_pool_.reserve(json_string.size());
  convertPtreeToPropertyTree(temp_ptree, property_tree_);

  return true;
}

void PropertyStore::convertPtreeToPropertyTree(const boost::property_tree::ptree& src,
                                               Common::PropertyTree& dest) {
  // The array detection: all keys are empty strings
  bool is_array = std::all_of(src.begin(), src.end(), [](const auto& pair) {
    if (pair.first.empty()) {
      return true;
    }
    return false;
  });

  if (is_array) {
    for (const auto& [_, child_tree] : src) {
      if (child_tree.empty()) {
        // Leaf node - convert the value
        dest.push_back(std::make_pair("", Common::PropertyTree(&dest)));
        Common::PropertyTree* new_tree = static_cast<Common::PropertyTree*>(&dest.back().second);
        new_tree->put("", convertValue(child_tree.data(), &dest));
      } else {
        // Branch node - recursively convert children
        dest.push_back(std::make_pair("", Common::PropertyTree(&dest)));
        Common::PropertyTree* new_tree = static_cast<Common::PropertyTree*>(&dest.back().second);
        convertPtreeToPropertyTree(child_tree, *new_tree);
      }
    }
  } else {
    for (const auto& [key, child_tree] : src) {
      if (child_tree.empty()) {
        // Leaf node - convert the value
        dest.put(key, convertValue(child_tree.data(), &dest));
      } else {
        // Branch node - recursively convert children
        Common::PropertyTree* new_tree =
            static_cast<Common::PropertyTree*>(&dest.put_child(key, Common::PropertyTree(&dest)));
        convertPtreeToPropertyTree(child_tree, *new_tree);
      }
    }
  }
}

Common::PropertyTreeValue PropertyStore::convertValue(const std::string& value,
                                                      Common::PropertyTree* parent) {
  if (value.empty()) {
    return {std::monostate{}, parent};
  }

  // Try to parse as boolean or null
  if (value == "true") {
    return {1, parent};
  } else if (value == "false") {
    return {0, parent};
  } else if (value == "null") {
    return {std::monostate{}, parent};
  }

  // Try to parse as number (integer or floating point)
  bool has_dot = false;
  bool is_valid_number = true;
  for (size_t i = 0; i < value.size() && is_valid_number; ++i) {
    const char& c = value[i];
    if (c == '.') {
      if (has_dot || i == 0 || i == value.size() - 1) {
        is_valid_number = false;
      } else {
        has_dot = true;
      }
    } else if (c == '-') {
      if (i != 0) {
        is_valid_number = false;
      }
    } else if (!std::isdigit(c)) {
      is_valid_number = false;
    }
  }
  if (is_valid_number) {
    if (has_dot) {
      // Parse as floating point and convert to integer (multiply by 100)
      double double_value = 0;
      std::from_chars(value.data(), value.data() + value.size(), double_value);
      return {static_cast<int64_t>(std::round(double_value * 100)), parent};
    } else {
      // Parse as integer
      int64_t int_value = 0;
      std::from_chars(value.data(), value.data() + value.size(), int_value);
      return {int_value, parent};
    }
  }

  string_pool_.append(value);
  return {std::string_view(string_pool_.data() + string_pool_.size() - value.size(), value.size()),
          parent};
}

} // namespace Common
} // namespace Wge