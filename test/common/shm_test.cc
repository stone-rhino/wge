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
#include <gtest/gtest.h>

#include "common/shm.hpp"

namespace Wge {
namespace Common {
class ShmTest : public ::testing::Test {};

TEST_F(ShmTest, basic) {
  Shm shm;

  int& val1 = shm.try_emplace<int>("key1", 42);
  int& val2 = shm.try_emplace<int>("key2", 84);

  EXPECT_EQ(val1, 42);
  EXPECT_EQ(val2, 84);

  int* find_val1 = shm.find<int>("key1");
  ASSERT_NE(find_val1, nullptr);
  EXPECT_EQ(*find_val1, 42);

  int* find_val2 = shm.find<int>("key2");
  ASSERT_NE(find_val2, nullptr);
  EXPECT_EQ(*find_val2, 84);

  int* find_val3 = shm.find<int>("key3");
  EXPECT_EQ(find_val3, nullptr);
}

TEST_F(ShmTest, string) {
  Shm shm;

  Shm::string& str1 = shm.try_emplace<Shm::string>("str_key1", "Hello, world!");
  Shm::string& str2 = shm.try_emplace<Shm::string>(
      "str_key2", "This is a test string. Testing string storage in shared memory.");

  EXPECT_EQ(str1, "Hello, world!");
  EXPECT_EQ(str2, "This is a test string. Testing string storage in shared memory.");

  Shm::string* find_str1 = shm.find<Shm::string>("str_key1");
  ASSERT_NE(find_str1, nullptr);
  EXPECT_EQ(*find_str1, "Hello, world!");

  Shm::string* find_str2 = shm.find<Shm::string>("str_key2");
  ASSERT_NE(find_str2, nullptr);
  EXPECT_EQ(*find_str2, "This is a test string. Testing string storage in shared memory.");

  Shm::string* find_str3 = shm.find<Shm::string>("str_key3");
  EXPECT_EQ(find_str3, nullptr);
}

TEST_F(ShmTest, vector) {
  Shm shm;

  Shm::vector<int>& vec = shm.try_emplace<Shm::vector<int>>("vec_key");
  vec.emplace_back(0);
  vec.emplace_back(1);
  vec.emplace_back(2);

  ASSERT_EQ(vec.size(), 3);
  EXPECT_EQ(vec[0], 0);
  EXPECT_EQ(vec[1], 1);
  EXPECT_EQ(vec[2], 2);
}

TEST_F(ShmTest, list) {
  Shm shm;

  Shm::list<int>& lst = shm.try_emplace<Shm::list<int>>("list_key");
  lst.emplace_back(0);
  lst.emplace_back(1);
  lst.emplace_back(2);

  ASSERT_EQ(lst.size(), 3);
  auto it = lst.begin();
  EXPECT_EQ(*it++, 0);
  EXPECT_EQ(*it++, 1);
  EXPECT_EQ(*it++, 2);
}

TEST_F(ShmTest, map) {
  Shm shm;

  Shm::map<Shm::string, int>& mp = shm.try_emplace<Shm::map<Shm::string, int>>("map_key");
  mp[shm.make_string("one")] = 1;
  mp[shm.make_string("two")] = 2;
  mp[shm.make_string("three")] = 3;

  ASSERT_EQ(mp.size(), 3);
  EXPECT_EQ(mp[shm.make_string("one")], 1);
  EXPECT_EQ(mp[shm.make_string("two")], 2);
  EXPECT_EQ(mp[shm.make_string("three")], 3);
}

TEST_F(ShmTest, set) {
  Shm shm;

  Shm::set<int>& st = shm.try_emplace<Shm::set<int>>("set_key");
  st.insert(1);
  st.insert(2);
  st.insert(3);

  ASSERT_EQ(st.size(), 3);
  EXPECT_NE(st.find(1), st.end());
  EXPECT_NE(st.find(2), st.end());
  EXPECT_NE(st.find(3), st.end());
}

TEST_F(ShmTest, sameName) {
  const char* shm_name = "test_shm_same_name";

  Shm shm1(shm_name);
  shm1.try_emplace<int>("key1", 123);

  Shm shm2(shm_name);
  int* find_val1 = shm2.find<int>("key1");
  ASSERT_NE(find_val1, nullptr);
  EXPECT_EQ(*find_val1, 123);
}

TEST_F(ShmTest, customType) {
  struct CustomType {
    int foo_;
    double bar_;
  };

  Shm shm;

  CustomType& custom_val = shm.try_emplace<CustomType>("custom_key");
  custom_val.foo_ = 10;
  custom_val.bar_ = 3.14;

  CustomType* find_custom_val = shm.find<CustomType>("custom_key");
  ASSERT_NE(find_custom_val, nullptr);
  EXPECT_EQ(find_custom_val->foo_, 10);
  EXPECT_DOUBLE_EQ(find_custom_val->bar_, 3.14);
}

TEST_F(ShmTest, crossProcess) {
  struct CustomType {
    Shm::vector<int> foo_;
    double bar_;
    CustomType(Shm::vector<int>&& foo, double bar) : foo_(std::move(foo)), bar_(bar) {}
  };

  const char* shm_name = "test_shm_cross_process";
  Shm shm(shm_name);

  CustomType& custom_val = shm.try_emplace<CustomType>("custom_key", shm.make_vector<int>(), 3.14);
  custom_val.foo_.emplace_back(1);
  custom_val.foo_.emplace_back(2);
  custom_val.foo_.emplace_back(3);

  CustomType* find_custom_val = shm.find<CustomType>("custom_key");
  ASSERT_NE(find_custom_val, nullptr);
  EXPECT_EQ(find_custom_val->foo_.size(), 3);
  EXPECT_EQ(find_custom_val->foo_[0], 1);
  EXPECT_EQ(find_custom_val->foo_[1], 2);
  EXPECT_EQ(find_custom_val->foo_[2], 3);
  EXPECT_DOUBLE_EQ(find_custom_val->bar_, 3.14);

  // Use fork to create a child process
  pid_t pid = fork();
  ASSERT_NE(pid, -1) << "Fork failed";

  if (pid == 0) {
    // Child process
    Shm shm(shm_name, Shm::default_size);
    CustomType* find_custom_val = shm.find<CustomType>("custom_key");
    ASSERT_NE(find_custom_val, nullptr);
    EXPECT_EQ(find_custom_val->foo_.size(), 3);
    EXPECT_EQ(find_custom_val->foo_[0], 1);
    EXPECT_EQ(find_custom_val->foo_[1], 2);
    EXPECT_EQ(find_custom_val->foo_[2], 3);
    EXPECT_DOUBLE_EQ(find_custom_val->bar_, 3.14);
    find_custom_val->foo_.emplace_back(4);
    find_custom_val->foo_.emplace_back(5);
    find_custom_val->foo_.emplace_back(6);
    find_custom_val->bar_ = 3.14159;

    exit(0);
  } else {
    // Parent process
    int status;
    waitpid(pid, &status, 0); // Wait for child process to finish

    // Check the updated values
    EXPECT_EQ(find_custom_val->foo_.size(), 6);
    EXPECT_EQ(find_custom_val->foo_[0], 1);
    EXPECT_EQ(find_custom_val->foo_[1], 2);
    EXPECT_EQ(find_custom_val->foo_[2], 3);
    EXPECT_EQ(find_custom_val->foo_[3], 4);
    EXPECT_EQ(find_custom_val->foo_[4], 5);
    EXPECT_EQ(find_custom_val->foo_[5], 6);
    EXPECT_DOUBLE_EQ(find_custom_val->bar_, 3.14159);
  }
}

TEST_F(ShmTest, crossProcessSynchronization) {
  struct CustomType {
    int foo_;
    int bar_;
    Shm::mutex foo_mutex_;
    Shm::mutex bar_mutex_;
    Shm::condition bar_cond_;
  };

  const char* shm_name = "test_shm_cross_process_sync";
  Shm shm(shm_name);

  auto& custom_val = shm.try_emplace<CustomType>("custom_key", 0, 0);
  Shm::scoped_lock lock(custom_val.bar_mutex_);

  // Use fork to create a child process
  pid_t pid = fork();
  const size_t test_count = 1000000;
  if (pid == 0) {
    pid_t pid2 = fork();
    if (pid2 == 0) {
      Shm shm(shm_name, Shm::default_size);
      CustomType* find_custom_val = shm.find<CustomType>("custom_key");
      {
        Shm::scoped_lock lock(find_custom_val->bar_mutex_);
        find_custom_val->bar_ = 16888;
        find_custom_val->bar_cond_.notify_all();
      }
      exit(0);
    } else {
      Shm shm(shm_name, Shm::default_size);
      CustomType* find_custom_val = shm.find<CustomType>("custom_key");
      for (size_t i = 0; i < test_count; ++i) {
        {
          Shm::scoped_lock lock(find_custom_val->foo_mutex_);
          find_custom_val->foo_ += 1;
        }
      }
      exit(0);
    }
  }

  for (size_t i = 0; i < test_count; ++i) {
    {
      Shm::scoped_lock lock(custom_val.foo_mutex_);
      custom_val.foo_ += 1;
    }
  }

  // Wait for child process1 to finish
  int status;
  waitpid(pid, &status, 0);

  // Test condition variable
  // Wait for child process2 to finish
  custom_val.bar_cond_.wait(lock);

  EXPECT_EQ(custom_val.foo_, test_count * 2);
  EXPECT_EQ(custom_val.bar_, 16888);
}

} // namespace Common
} // namespace Wge
