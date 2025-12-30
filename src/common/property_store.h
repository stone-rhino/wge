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

#include <expected>
#include <string>

#include "property_tree.h"

namespace Wge {
namespace Common {
/**
 * PropertyStore class manages a PropertyTree and its associated string memory pool.
 */
class PropertyStore {
public:
  PropertyStore() = default;
  PropertyStore(const PropertyStore&) = delete;
  PropertyStore& operator=(const PropertyStore&) = delete;
  PropertyStore(PropertyStore&&) = default;
  PropertyStore& operator=(PropertyStore&&) = default;

public:
  /**
   * Constructs a PropertyStore from a JSON string.
   * @param json_string JSON string representing the property tree
   * @return an expected object containing true if successful, or an error string if failed
   * @note The json values are stored in Common::Variant as follows:
   * - string: string_view
   * - number: int64_t (multiplied by 100 if it is a floating point number)
   * - boolean: int64_t (1 for true, 0 for false)
   * - null: std::monostate
   */
  std::expected<bool, std::string> loadFromJsonString(const std::string& json_string);

  /**
   * Clears the property tree and the string pool.
   */
  void clear() {
    property_tree_.clear();
    string_pool_.clear();
  }

  /**
   * Gets the property tree.
   * @return reference to the PropertyTree
   */
  PropertyTree& getPropertyTree() { return property_tree_; }

  /**
   * Gets the property tree (const version).
   * @return const reference to the PropertyTree
   */
  const PropertyTree& getPropertyTree() const { return property_tree_; }

private:
  void convertPtreeToPropertyTree(const boost::property_tree::ptree& src,
                                  Common::PropertyTree& dest);
  Common::PropertyTreeValue convertValue(const std::string& value, Common::PropertyTree* parent);

private:
  PropertyTree property_tree_;
  std::string string_pool_;
};

} // namespace Common
} // namespace Wge