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

#include <functional>
#include <mutex>
#include <string>
#include <unordered_map>

#include <stdint.h>

#include "../common/evaluate_result.h"

namespace Wge {
namespace PersistentStorage {
class Collection {
public:
  Collection();

public:
  /**
   * Set a key/value pair in the collection.
   * @param key The key to set
   * @param value The value to set
   */
  void set(const std::string& key, const Common::Variant& value);

  /**
   * Get a value from the collection by key.
   * @param key Specifies the key to get
   * @return The value associated with the key
   */
  const Common::Variant& get(const std::string& key) const;

  /**
   * Get the number of key/value pairs in the collection.
   * @return The number of key/value pairs in the collection.
   */
  size_t size() const { return kv_.size(); }

  /**
   * Iterate over all key/value pairs in the collection.
   * @param func A function to call for each key/value pair. The function should return true to
   * continue iterating, or false to stop.
   */
  void travel(std::function<bool(const std::string&, const Common::Variant&)> func) const;

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
   * @return The value of the initcol variable
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

  std::unordered_map<std::string, Common::EvaluateElement> kv_;
  mutable std::mutex kv_mutex_;
};
} // namespace PersistentStorage
} // namespace Wge