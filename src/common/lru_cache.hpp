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
#include <functional>
#include <list>
#include <mutex>

#include "hash_table.hpp"

namespace SrSecurity {
namespace Common {
/**
 * Use LRU(Least Recently Used) algorithm to clean the cache.
 * All methods of this class are thread-safe.
 * @tparam KEY the type of key
 * @tparam VALUE the type of value
 * @tparam hash_table_slot_count the count of hash table slot, must be a prime number
 */
template <typename KEY, typename VALUE, size_t hash_table_slot_count = 8191> class LruCache {
public:
  LruCache(size_t max_size = 1024 * 16) : max_size_(max_size > 0 ? max_size : 1) {}
  LruCache(const LruCache& cache) = delete;

private:
  struct Node;
  using HashTableType = HashTable<KEY, typename std::list<Node>::iterator, hash_table_slot_count>;

public:
  /**
   * Get the cache of the specified key and process the node. If the cache of the specified key does
   * not exist, add it. After calling this method, the corresponding node is moved to the cache
   * head.
   * @param key the specified key
   * @param found the operation to be performed after finding (adding) the cache of the specified
   * key.
   * @param value_factory_cb when the cache of the specified key does not exist, use this factory
   * function to generate a new node.
   */
  void access(const KEY& key, std::function<void(VALUE&)> found,
              std::function<VALUE()> value_factory_cb) {
    map_.readLock(key);

    // Find the node and process it
    if (lookup(key, found)) {
      map_.readUnlock(key);
      return;
    }

    if (!value_factory_cb) {
      return;
    }

    // Upgrade to write lock, this operation is not atomic, need to verify again whether it exists
    map_.readUnlock(key);
    map_.writeLock(key);
    if (lookup(key, found)) {
      map_.writeUnlock(key);
      return;
    }

    // Add node and process
    mutex_.lock();
    Node node(value_factory_cb());
    cache_.emplace_front(std::move(node));
    auto cache_iter = cache_.begin();
    cache_iter->table_iter = map_.insert(key, cache_iter);
    if (found) {
      found(cache_iter->value);
    }
    mutex_.unlock();

    map_.writeUnlock(key);

    tryClean(1);
  }

  /**
   * Peek at the cache of the specified key and process the node.
   * After calling this method, the cache will not be changed: if the node does not exist, a new
   * node will not be added; if the node exists, it will not be moved to the cache head.
   * @param key the specified key
   * @param found the operation to be performed after finding (adding) the cache of the specified
   * key.
   */
  void peek(const KEY& key, std::function<void(const VALUE*)> found) {
    if (!found) {
      return;
    }

    map_.readLock(key);

    auto iter = map_.find(key);
    if (iter != map_.end(key)) {
      mutex_.lock();
      found(&(iter->value->value));
      mutex_.unlock();
    } else {
      found(nullptr);
    }

    map_.readUnlock(key);
  }

  /**
   * Peek at the cache of the specified key and process the node.
   * After calling this method, the cache will not be changed: if the node does not exist, a new
   * node will not be added; if the node exists, it will not be moved to the cache head.
   * @param pos the position of the cache node to be peeked. base 0
   * @param found the operation to be performed after finding (adding) the cache of the specified
   * key. if the node does not exist, const VALUE* == nullptr
   */
  void peek(size_t pos, std::function<void(const VALUE*)> found) {
    if (!found) {
      return;
    }

    mutex_.lock();

    auto iter = cache_.begin();
    for (size_t i = 0; i < pos; i++) {
      ++iter;
    }

    if (iter != cache_.end()) {
      found(&(iter->value));
    } else {
      found(nullptr);
    }

    mutex_.unlock();
  }

  /**
   * Clear the cache.
   * @param count the count of nodes to be cleaned
   * @param predicat the condition for cleaning nodes. return false to stop cleaning, otherwise
   * continue cleaning.
   */
  void clean(size_t count, std::function<bool(const VALUE& value)> predicat) {
    mutex_.lock();

    for (size_t i = 0; i < count; ++i) {
      if (cache_.empty()) {
        break;
      }

      if (!predicat(cache_.back().value)) {
        break;
      }

      const size_t slot_index = cache_.back().table_iter.slotIndex();
      if (!map_.tryWriteLock(slot_index)) {
        break;
      }

      map_.erase(cache_.rbegin()->table_iter);
      cache_.pop_back();
      map_.writeUnlock(slot_index);
    }

    mutex_.unlock();
  }

  /**
   * Get the size of the cache.
   * @return the size of the cache
   */
  size_t size() {
    std::lock_guard<std::mutex> locker(mutex_);
    return cache_.size();
  }

  size_t maxSize() const { return max_size_; }

private:
  // The type of cache node
  struct Node {
    Node(VALUE&& v) : value(std::forward<VALUE>(v)) {}

    typename HashTableType::iterator table_iter;
    VALUE value;
  };

  std::list<Node> cache_;
  HashTableType map_;
  std::mutex mutex_;
  size_t max_size_;

private:
  // Find the node and process it
  bool lookup(const KEY& key, std::function<void(VALUE&)> found) {
    auto iter = map_.find(key);
    if (iter != map_.end(key)) {
      mutex_.lock();
      cache_.splice(cache_.begin(), cache_, iter->value);
      if (found) {
        found(iter->value->value);
      }
      mutex_.unlock();

      return true;
    }

    return false;
  }

  // Clean the cache, only called after adding a node
  void tryClean(size_t count) {
    if (cache_.size() <= max_size_) {
      return;
    }

    mutex_.lock();

    if (cache_.size() > max_size_) {
      for (size_t i = 0; i < count; ++i) {
        const size_t slot_index = cache_.back().table_iter.slotIndex();
        if (!map_.tryWriteLock(slot_index)) {
          break;
        }

        map_.erase(cache_.rbegin()->table_iter);
        cache_.pop_back();
        map_.writeUnlock(slot_index);
      }
    }

    mutex_.unlock();
  }
};
} // namespace Common
} // namespace SrSecurity