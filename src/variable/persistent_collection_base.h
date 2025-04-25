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

#include "collection_base.h"

#include "../persistent_storage/storage.h"
#include "../transaction.h"

namespace Wge {
namespace Variable {
class PersistentCollectionBase : public CollectionBase {

public:
  PersistentCollectionBase(PersistentStorage::Storage::Type type, const std::string& sub_name)
      : type_(type), CollectionBase(sub_name) {}

public:
  size_t size(Transaction& t) const {
    const std::string& collection_name = t.getPersistentStorageKey(type_);
    auto collection = t.getEngine().storage().collection(type_, collection_name);
    if (collection) {
      return collection->size();
    } else {
      return 0;
    }
  }

  void travel(Transaction& t,
              std::function<bool(const std::string&, const Common::Variant&)> func) const {
    const std::string& collection_name = t.getPersistentStorageKey(type_);
    auto collection = t.getEngine().storage().collection(type_, collection_name);
    if (collection) {
      collection->travel(func);
    }
  }

  const Common::Variant& get(Transaction& t, const std::string& key) const {
    const std::string& collection_name = t.getPersistentStorageKey(type_);
    auto collection = t.getEngine().storage().collection(type_, collection_name);
    if (collection) {
      return collection->get(key);
    } else {
      return EMPTY_VARIANT;
    }
  }

private:
  PersistentStorage::Storage::Type type_;
};
} // namespace Variable
} // namespace Wge