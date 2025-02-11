#pragma once

#include <unordered_map>

#include "collection.h"

namespace SrSecurity {
namespace PersistentStorage {
class Storage {
public:
  void loadFromFile(const std::string& file);
  void storeToFile(const std::string& file);

public:
  void initCollection(std::string&& collection_name);
  Collection* collection(std::string&& collection_name);

private:
  std::unordered_map<std::string, Collection> collections_;
};
} // namespace PersistentStorage
} // namespace SrSecurity