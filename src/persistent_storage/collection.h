#pragma once

#include <string>
#include <unordered_map>

#include <stdint.h>

namespace SrSecurity {
namespace PersistentStorage {
class Collection {
public:
  Collection();

public:
  void set(const std::string& key, std::string&& value);
  const std::string* get(const std::string& key) const;

  // Built-in attributes
public:
  /**
   * Get timestamp of the creation of the collection.
   * @return The number of seconds since 1970/1/1
   */
  time_t createTime() const;

  /**
   * Check whether the collection is new.
   * @return Ture if the collection is new (not yet persisted) otherwise returns false.
   */
  bool isNew() const;

  /**
   * @return The value of the initcol variable (the client's IP address in the example).
   */
  const std::string& key() const;

  /**
   * Get timestamp of the last update to the collection.
   * @return The number of seconds since 1970/1/1
   */
  time_t lastUpdateTime() const;

  /**
   * date/time in seconds when the collection will be updated on disk from memory (if no other
   * updates occur). This variable may be set if you wish to specifiy an explicit expiration time
   * (default is 3600 seconds). The TIMEOUT is updated every time that the values of an entry is
   * changed.
   */
  uint32_t timeout() const;

  /**
   * @return How many times the collection has been updated since creation.
   */
  uint64_t updateCounter() const;

  /**
   * @return The average rate updates per minute since creation.
   */
  uint64_t updateRate() const;

private:
  time_t create_time_;
  bool is_new_{false};
  time_t last_update_time_;
  uint32_t timeout_{3600};
  uint64_t update_counter_;

  std::unordered_map<std::string, std::string> kv_;
};
} // namespace PersistentStorage
} // namespace SrSecurity