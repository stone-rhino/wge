#pragma once

#include <array>
#include <memory>
#include <string_view>

#include "hs_database.h"

#include "../pcre/scanner.h"

namespace SrSecurity {
namespace Common {
namespace Hyperscan {
class Scanner {
public:
  Scanner(const std::shared_ptr<HsDataBase> hs_db);

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
  void registMatchCallback(MatchCallback cb, void* user_data);
  void registPcreRemoveDuplicateCallback(PcreRemoveDuplicateCallbak cb, void* user_data);
  void blockScan(std::string_view data);
  void streamScanStart();
  void streamScan(std::string_view data);
  void streamScanStop();

private:
private:
  static int matchCallback(unsigned int id, unsigned long long from, unsigned long long to,
                           unsigned int flags, void* user_data);

private:
  static thread_local std::unique_ptr<Scratch> worker_scratch_;
  const std::shared_ptr<HsDataBase> hs_db_;
  hs_stream_t* stream_id_{nullptr};
  MatchCallback match_cb_{nullptr};
  void* match_cb_user_data_;
  PcreRemoveDuplicateCallbak pcre_remove_duplicate_cb_{nullptr};
  void* pcre_remove_duplicate_cb_user_data_;
  std::string_view curr_match_data_;

  // pcre
  std::unique_ptr<Pcre::Scanner> pcre_;
};
} // namespace Hyperscan
} // namespace Common
} // namespace SrSecurity