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
   * @param stream_db: stream mode database
   */
  void add(const hs_database_t* block_db, const hs_database_t* stream_db) {
    if (block_db && !block_scratch_) {
      ::hs_alloc_scratch(block_db, &block_scratch_);
    }
    if (stream_db && !stream_scratch_) {
      ::hs_alloc_scratch(stream_db, &stream_scratch_);
    }
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