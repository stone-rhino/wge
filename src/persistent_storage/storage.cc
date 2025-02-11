#include "storage.h"

namespace SrSecurity {
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
  auto iter = collections_.find(collection_name);
  if (iter == collections_.end()) {
    collections_.emplace(collection_name, Collection());
  }
}

Collection* Storage::collection(std::string&& collection_name) {
  auto iter = collections_.find(collection_name);
  if (iter != collections_.end()) {
    return &iter->second;
  }

  return nullptr;
}

} // namespace PersistentStorage
} // namespace SrSecurity