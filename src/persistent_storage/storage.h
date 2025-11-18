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

#include <array>
#include <mutex>

#include "collection.h"

namespace Wge {
namespace PersistentStorage {
/**
 * The persistent storage is used to store the data that needs to be persisted across requests.
 * At this time it is only possible to have five types of collections: GLOBAL, RESOURCE, IP,
 * SESSION, USER.
 * The internal data structure is as follows:
 * array
 * +---------+-----------+---------+----------+---------+
 * |  GLOBAL |  RESOURCE |   IP    |  SESSION |  USER   |
 * +---------+-----------+---------+----------+---------+
 *                 hash table: ↓
 *                 +-------+-------+-------+
 *                 | key1  |  key2 |  ...  |
 *                 +-------+-------+-------+
 *                 Collection: ↓
 *                 +-------+-------+-------+
 *                 | key1  |  key2 |  ...  |
 *                 +-------+-------+-------+
 *                      value: ↓
 *                      +---------------+
 *                      | string or int |
 *                      +---------------+
 */
class Storage {
public:
  enum class Type { GLOBAL = 0, RESOURCE, IP, SESSION, USER, SizeOfType };

public:
  void loadFromFile(const std::string& file);
  void storeToFile(const std::string& file);

public:
  void initCollection(Type type, std::string_view collection_name);
  Collection* collection(Type type, std::string_view collection_name);

private:
  std::array<std::unordered_map<std::string_view, Collection>,
             static_cast<size_t>(Type::SizeOfType)>
      collections_;
  std::array<std::mutex, static_cast<size_t>(Type::SizeOfType)> collections_mutex_;
};
} // namespace PersistentStorage
} // namespace Wge