#include "collection.h"

#include <time.h>

namespace SrSecurity {
namespace PersistentStorage {
Collection::Collection() { create_time_ = ::time(nullptr); }

void Collection::set(const std::string& key, std::string&& value) {
  kv_[key] = std::move(value);
  last_update_time_ = ::time(nullptr);
  ++update_counter_;
}

const std::string* Collection::get(const std::string& key) const {
  auto iter = kv_.find(key);
  if (iter != nullptr) {
    return &iter->second;
  }

  return nullptr;
}
} // namespace PersistentStorage
} // namespace SrSecurity