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
#include "storage.h"

namespace Wge {
namespace PersistentStorage {

void Storage::loadFromFile(const std::string& file) {
  // TODO: Implement
  throw "Not implemented!";
}

void Storage::storeToFile(const std::string& file) {
  // TODO: Implement
  throw "Not implemented!";
}

void Storage::initCollection(std::string&& collection_name) {
  std::lock_guard<std::mutex> lock(collections_mutex_);
  collections_.try_emplace(std::move(collection_name));
}

Collection* Storage::collection(const std::string& collection_name) {
  std::lock_guard<std::mutex> lock(collections_mutex_);
  auto iter = collections_.find(collection_name);
  if (iter != collections_.end()) {
    return &iter->second;
  }

  return nullptr;
}

} // namespace PersistentStorage
} // namespace Wge