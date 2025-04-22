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

#include <hs/hs.h>

namespace SrSecurity {
namespace Common {
namespace Hyperscan {
/**
 * Scratch space for use by Hyperscan.
 * Only one scratch space is required per thread, even when using multiple databases.
 */
class Scratch {
public:
  Scratch() = default;
  ~Scratch() { free(); }

  /**
   * Copy constructor.
   * In a scenario where a database is created by main thread and data will be scanned by multiple
   * worker threads, the copy constructor is used to create a new scratch space for each worker
   * thread(rather than forcing us to pass all the databases through add() multiple times).
   */
  Scratch(const Scratch& scratch) {
    ::hs_clone_scratch(scratch.block_scratch_, &block_scratch_);
    ::hs_clone_scratch(scratch.stream_scratch_, &stream_scratch_);
  }

public:
  /**
   * Add scratch space for the given databases.
   * If we uses multiple databases, only a single scratch space is needed: in this case, call this
   * function for each database.
   * @param block_db: block mode database
   */
  void addBlock(const hs_database_t* block_db) { ::hs_alloc_scratch(block_db, &block_scratch_); }

  /**
   * Add scratch space for the given databases.
   * If we uses multiple databases, only a single scratch space is needed: in this case, call this
   * function for each database.
   * @param stream_db: stream mode database
   */
  void addStream(const hs_database_t* stream_db) {
    ::hs_alloc_scratch(stream_db, &stream_scratch_);
  }

  /**
   * Free the scratch space.
   */
  void free() {
    if (block_scratch_) {
      ::hs_free_scratch(block_scratch_);
    }
    if (stream_scratch_) {
      ::hs_free_scratch(stream_scratch_);
    }
  }

public:
  /**
   * Callback function for hyperscan match
   * @param id the pattern id
   * @param from the start offset of the match
   * @param to the end offset of the match
   * @param flags the flags
   * @param user_data the user data
   * @return 0 to continue, non-zero to stop
   */
  using MatchCallback = int (*)(uint64_t id, unsigned long long from, unsigned long long to,
                                unsigned int flags, void* user_data);
  using PcreRemoveDuplicateCallbak = bool (*)(uint64_t id, unsigned long long to, void* user_data);

public:
  hs_scratch_t* block_scratch_{nullptr};
  hs_scratch_t* stream_scratch_{nullptr};
  hs_stream_t* stream_id_{nullptr};
  std::string_view curr_match_data_;

  MatchCallback match_cb_{nullptr};
  void* match_cb_user_data_;
  PcreRemoveDuplicateCallbak pcre_remove_duplicate_cb_{nullptr};
  void* pcre_remove_duplicate_cb_user_data_;
};
} // namespace Hyperscan
} // namespace Common
} // namespace SrSecurity