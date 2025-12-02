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

#include <atomic>
#include <chrono>
#include <format>

#include <boost/interprocess/containers/list.hpp>
#include <boost/interprocess/containers/map.hpp>
#include <boost/interprocess/containers/set.hpp>
#include <boost/interprocess/containers/string.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/sync/interprocess_condition.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>

namespace Wge {
namespace Common {
namespace bi = boost::interprocess;

/**
 * Shared memory manager
 * Provides STL-like containers and POD types in shared memory.
 * Use RAII to manage shared memory lifetime
 * @note All objects in shared memory must be POD or use containers provided by this class.
 * Otherwise, may be cause memory corruption or leaks.
 */
class Shm {
public:
  static constexpr size_t default_size = 1024 * 64;

  // Type aliases for STL-like containers in shared memory
public:
  template <class T> using allocator = bi::allocator<T, bi::managed_shared_memory::segment_manager>;

  template <class T> using basic_string = bi::basic_string<T, std::char_traits<T>, allocator<T>>;

  using string = basic_string<char>;

  template <class T> using vector = bi::vector<T, allocator<T>>;

  template <class T> using list = bi::list<T, allocator<T>>;

  template <class K, class V>
  using map = bi::map<K, V, std::less<K>, allocator<std::pair<const K, V>>>;

  template <class K> using set = bi::set<K, std::less<K>, allocator<K>>;

  // Synchronization primitives in shared memory
public:
  using mutex = bi::interprocess_mutex;
  using scoped_lock = bi::scoped_lock<mutex>;
  using condition = bi::interprocess_condition;

public:
  /**
   * Generate a unique shared memory name
   * @return Unique shared memory name
   */
  static std::string generateShmName() {
    using namespace std::chrono;
    static std::atomic<int> counter = 0;
    uint64_t timestamp =
        time_point_cast<std::chrono::milliseconds>(system_clock::now()).time_since_epoch().count();
    return std::format("wge_shm_{}_{}_{}", ::getpid(), counter++, timestamp);
  }

public:
  /**
   * Constructor
   * @param name Shared memory name
   * @param size Shared memory size. Default is 64KB
   */
  Shm(const char* name = generateShmName().c_str(), size_t size = default_size)
      : name_(name), segment_(boost::interprocess::open_or_create, name_.c_str(), size) {}

  ~Shm() { boost::interprocess::shared_memory_object::remove(name_.c_str()); }

public:
  /**
   * Emplace an object in shared memory or get existing object
   * @tparam T Object type
   * @tparam Args Constructor arguments
   * @param key Object key
   * @param args Constructor arguments
   * @return Reference to the emplaced object or existing object
   */
  template <class T, class... Args> T& try_emplace(const char* key, Args&&... args) {
    if constexpr (needs_allocator<T>::value) {
      return *segment_.find_or_construct<T>(key)(std::forward<Args>(args)...,
                                                 segment_.get_segment_manager());
    } else {
      return *segment_.find_or_construct<T>(key)(std::forward<Args>(args)...);
    }
  }

  /**
   * Find an object in shared memory
   * @tparam T Object type
   * @param key Object key
   * @return Pointer to the object, or nullptr if not found
   */
  template <class T> T* find(const char* key) { return segment_.find<T>(key).first; }

  /**
   * Create STL-like string in shared memory
   * @tparam Args Constructor arguments
   * @param args Constructor arguments
   * @return Shared memory string
   */
  template <class... Args> string make_string(Args&&... args) const {
    return string(std::forward<Args>(args)..., segment_.get_segment_manager());
  }

  /**
   * Create STL-like vector in shared memory
   * @tparam T Element type
   * @tparam Args Constructor arguments
   * @param args Constructor arguments
   * @return Shared memory vector
   */
  template <class T, class... Args> vector<T> make_vector(Args&&... args) const {
    return vector<T>(std::forward<Args>(args)..., segment_.get_segment_manager());
  }

  /**
   * Create STL-like list in shared memory
   * @tparam T Element type
   * @tparam Args Constructor arguments
   * @param args Constructor arguments
   * @return Shared memory list
   */
  template <class T, class... Args> list<T> make_list(Args&&... args) const {
    return list<T>(std::forward<Args>(args)..., segment_.get_segment_manager());
  }

  /**
   * Create STL-like map in shared memory
   * @tparam K Key type
   * @tparam V Value type
   * @tparam Args Constructor arguments
   * @param args Constructor arguments
   * @return Shared memory map
   */
  template <class K, class V, class... Args> map<K, V> make_map(Args&&... args) const {
    return map<K, V>(std::forward<Args>(args)..., std::less<K>(), segment_.get_segment_manager());
  }

  /**
   * Create STL-like set in shared memory
   * @tparam K Element type
   * @tparam Args Constructor arguments
   * @param args Constructor arguments
   * @return Shared memory set
   */
  template <class K, class... Args> set<K> make_set(Args&&... args) const {
    return set<K>(std::forward<Args>(args)..., std::less<K>(), segment_.get_segment_manager());
  }

private:
  template <class T> struct needs_allocator : std::false_type {};
  template <class T> struct needs_allocator<basic_string<T>> : std::true_type {};
  template <class T> struct needs_allocator<vector<T>> : std::true_type {};
  template <class T> struct needs_allocator<list<T>> : std::true_type {};
  template <class K, class V> struct needs_allocator<map<K, V>> : std::true_type {};
  template <class K> struct needs_allocator<set<K>> : std::true_type {};

private:
  std::string name_;
  boost::interprocess::managed_shared_memory segment_;
};
} // namespace Common
} // namespace Wge