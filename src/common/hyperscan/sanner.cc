#include "scanner.h"

#include "../log.h"

namespace SrSecurity {
namespace Common {
namespace Hyperscan {
thread_local std::unique_ptr<Scratch> Scanner::worker_scratch_;

Scanner::Scanner(const std::shared_ptr<HsDataBase> hs_db) : hs_db_(hs_db) {
  // clone the main scratch space
  if (!worker_scratch_) [[unlikely]] {
    worker_scratch_ = std::make_unique<Scratch>(hs_db->mainScratch());
  }

  pcre_ = std::make_unique<Pcre::Scanner>(&hs_db_->getPcrePatternList());
}

void Scanner::registMatchCallback(MatchCallback cb, void* user_data) {
  match_cb_ = cb;
  match_cb_user_data_ = user_data;
}

void Scanner::registPcreRemoveDuplicateCallback(PcreRemoveDuplicateCallbak cb, void* user_data) {
  pcre_remove_duplicate_cb_ = cb;
  pcre_remove_duplicate_cb_user_data_ = user_data;
}

void Scanner::blockScan(std::string_view data) {
  if (!data.empty()) [[likely]] {
    curr_match_data_ = data;
    hs_error_t err = ::hs_scan(hs_db_->blockNative(), data.data(), data.length(), 0,
                               worker_scratch_->blockNative(), matchCallback, this);
    if (err != HS_SUCCESS) [[unlikely]] {
      SRSECURITY_LOG_ERROR("block mode hs_scan error");
    }
  }
}

void Scanner::streamScanStart() {
  assert(stream_id_ == nullptr);

  ::hs_open_stream(hs_db_->streamNative(), 0, &stream_id_);
}

void Scanner::streamScan(std::string_view data) {
  if (!data.empty()) [[likely]] {
    curr_match_data_ = data;
    hs_error_t err = ::hs_scan_stream(stream_id_, data.data(), data.length(), 0,
                                      worker_scratch_->streamNative(), matchCallback, this);
    if (err != HS_SUCCESS) [[unlikely]] {
      SRSECURITY_LOG_ERROR("stream mode hs_scan_stream error");
    }
  }
}

void Scanner::streamScanStop() {
  ::hs_close_stream(stream_id_, worker_scratch_->streamNative(), matchCallback, this);
}

int Scanner::matchCallback(unsigned int id, unsigned long long from, unsigned long long to,
                           unsigned int flags, void* user_data) {
  Scanner* parent = reinterpret_cast<Scanner*>(user_data);
  assert(parent);
  assert(parent->match_cb_);

  uint64_t real_id = parent->hs_db_->getRealId(id);

  // Try again with the pcre
  auto& pcre = *(parent->pcre_);
  auto pcre_pattern = pcre.getPattern(real_id);
  if (pcre_pattern) [[unlikely]] {
    // Format of the data to be scanned by pcre:
    // +----------------------------------+---------------------------------+
    // to - max_pcre_scan_front_len       to                 to + max_pcre_scan_back_len
    // note:
    // As you can see from the above solution, there is
    // a possibility of duplicate matching results, because a few bytes are retraced each time.
    // To solve such problems, we need remove duplicate information based on the from and to values.
    constexpr unsigned long long max_pcre_scan_front_len = 512;
    constexpr unsigned long long max_pcre_scan_back_len = 256;

    // ensure that matched info does not duplicate
    // remove duplicate information based on the from and to values.
    bool match_global = false;
    if (parent->pcre_remove_duplicate_cb_) [[unlikely]] {
      match_global = true;
      if (parent->pcre_remove_duplicate_cb_(real_id, to,
                                            parent->pcre_remove_duplicate_cb_user_data_)) {
        return 0;
      }
    }

    unsigned long long pcre_scan_from =
        to > max_pcre_scan_front_len ? to - max_pcre_scan_front_len : 0;
    unsigned long long pcre_scan_to =
        to + max_pcre_scan_back_len < parent->curr_match_data_.length()
            ? to + max_pcre_scan_back_len
            : parent->curr_match_data_.length();
    std::string_view pcre_scan_data =
        parent->curr_match_data_.substr(pcre_scan_from, pcre_scan_to - pcre_scan_from);

    if (match_global) {
      std::vector<std::pair<size_t, size_t>> pcre_match_info;
      pcre.matchGlobal(pcre_pattern, pcre_scan_data, pcre_match_info);
      for (const auto& [ovector_from, ovector_to] : pcre_match_info) {
        // Initialize the from and to values
        from = pcre_scan_from + ovector_from;
        to = from + (ovector_to - ovector_from);

        // Ensure that matched info does not duplicate
        // Remove duplicate information based on the from and to values.
        if (parent->pcre_remove_duplicate_cb_) {
          if (parent->pcre_remove_duplicate_cb_(real_id, to,
                                                parent->pcre_remove_duplicate_cb_user_data_)) {
            continue;
          }
        }

        // Notify matched
        parent->match_cb_(real_id, from, to, flags, parent->match_cb_user_data_);
      }
    } else {
      uint64_t new_from, new_to;
      std::vector<std::pair<size_t, size_t>> result;
      pcre.match(pcre_pattern, pcre_scan_data, result);
      if (!result.empty()) [[likely]] {
        // Recalculate the offset
        size_t new_from = result.front().first;
        size_t new_to = result.front().second;
        from = pcre_scan_from + new_from;
        to = from + (new_to - new_from);
        parent->match_cb_(real_id, from, to, flags, parent->match_cb_user_data_);
      }
    }
  } else {
    parent->match_cb_(real_id, from, to, flags, parent->match_cb_user_data_);
  }

  return 0;
}
} // namespace Hyperscan
} // namespace Common
} // namespace SrSecurity