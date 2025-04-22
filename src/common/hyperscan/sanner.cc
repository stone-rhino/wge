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
#include "scanner.h"

#include "../log.h"

namespace SrSecurity {
namespace Common {
namespace Hyperscan {
thread_local std::unique_ptr<Scratch> Scanner::worker_scratch_;

Scanner::Scanner(const std::shared_ptr<HsDataBase> hs_db) : hs_db_(hs_db) {
  pcre_ = std::make_unique<Pcre::Scanner>(&hs_db_->getPcrePatternList());
}

void Scanner::registMatchCallback(Scratch::MatchCallback cb, void* user_data) const {
  // clone the main scratch space
  if (!worker_scratch_) [[unlikely]] {
    worker_scratch_ = std::make_unique<Scratch>(hs_db_->mainScratch());
  }

  worker_scratch_->match_cb_ = cb;
  worker_scratch_->match_cb_user_data_ = user_data;
}

void Scanner::registPcreRemoveDuplicateCallback(Scratch::PcreRemoveDuplicateCallbak cb,
                                                void* user_data) const {
  // clone the main scratch space
  if (!worker_scratch_) [[unlikely]] {
    worker_scratch_ = std::make_unique<Scratch>(hs_db_->mainScratch());
  }

  worker_scratch_->pcre_remove_duplicate_cb_ = cb;
  worker_scratch_->pcre_remove_duplicate_cb_user_data_ = user_data;
}

void Scanner::blockScan(std::string_view data) const {
  // clone the main scratch space
  if (!worker_scratch_) [[unlikely]] {
    worker_scratch_ = std::make_unique<Scratch>(hs_db_->mainScratch());
  }

  if (!data.empty()) [[likely]] {
    worker_scratch_->curr_match_data_ = data;
    hs_error_t err =
        ::hs_scan(hs_db_->blockNative(), data.data(), data.length(), 0,
                  worker_scratch_->block_scratch_, matchCallback, const_cast<Scanner*>(this));
    if (err != HS_SUCCESS && err != HS_SCAN_TERMINATED) [[unlikely]] {
      SRSECURITY_LOG_ERROR("block mode hs_scan error");
    }
  }
}

void Scanner::streamScanStart() const {
  // clone the main scratch space
  if (!worker_scratch_) [[unlikely]] {
    worker_scratch_ = std::make_unique<Scratch>(hs_db_->mainScratch());
  }

  assert(worker_scratch_->stream_id_ == nullptr);
  ::hs_open_stream(hs_db_->streamNative(), 0, &worker_scratch_->stream_id_);
}

void Scanner::streamScan(std::string_view data) const {
  if (!data.empty()) [[likely]] {
    worker_scratch_->curr_match_data_ = data;
    hs_error_t err = ::hs_scan_stream(worker_scratch_->stream_id_, data.data(), data.length(), 0,
                                      worker_scratch_->stream_scratch_, matchCallback,
                                      const_cast<Scanner*>(this));
    if (err != HS_SUCCESS && err != HS_SCAN_TERMINATED) [[unlikely]] {
      SRSECURITY_LOG_ERROR("stream mode hs_scan_stream error");
    }
  }
}

void Scanner::streamScanStop() const {
  ::hs_close_stream(worker_scratch_->stream_id_, worker_scratch_->stream_scratch_, matchCallback,
                    const_cast<Scanner*>(this));
  worker_scratch_->stream_id_ = nullptr;
}

int Scanner::matchCallback(unsigned int id, unsigned long long from, unsigned long long to,
                           unsigned int flags, void* user_data) {
  const Scanner* scanner = reinterpret_cast<const Scanner*>(user_data);
  assert(scanner);
  assert(worker_scratch_->match_cb_);

  uint64_t real_id = scanner->hs_db_->getRealId(id);
  int cease = 0;

  // Try again with the pcre
  // Although the pcre scanner is member of the hyperscan scanner, but it is not stateful means that
  // it is thread safe. We use it directly.
  auto& pcre = *(scanner->pcre_);
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
    if (worker_scratch_->pcre_remove_duplicate_cb_) [[unlikely]] {
      match_global = true;
      if (worker_scratch_->pcre_remove_duplicate_cb_(
              real_id, to, worker_scratch_->pcre_remove_duplicate_cb_user_data_)) {
        return 0;
      }
    }

    unsigned long long pcre_scan_from =
        to > max_pcre_scan_front_len ? to - max_pcre_scan_front_len : 0;
    unsigned long long pcre_scan_to =
        to + max_pcre_scan_back_len < worker_scratch_->curr_match_data_.length()
            ? to + max_pcre_scan_back_len
            : worker_scratch_->curr_match_data_.length();
    std::string_view pcre_scan_data =
        worker_scratch_->curr_match_data_.substr(pcre_scan_from, pcre_scan_to - pcre_scan_from);

    if (match_global) {
      std::vector<std::pair<size_t, size_t>> pcre_match_info;
      pcre.matchGlobal(pcre_pattern, pcre_scan_data, pcre_match_info);
      for (const auto& [ovector_from, ovector_to] : pcre_match_info) {
        // Initialize the from and to values
        from = pcre_scan_from + ovector_from;
        to = from + (ovector_to - ovector_from);

        // Ensure that matched info does not duplicate
        // Remove duplicate information based on the from and to values.
        if (worker_scratch_->pcre_remove_duplicate_cb_) {
          if (worker_scratch_->pcre_remove_duplicate_cb_(
                  real_id, to, worker_scratch_->pcre_remove_duplicate_cb_user_data_)) {
            continue;
          }
        }

        // Notify matched
        cease = worker_scratch_->match_cb_(real_id, from, to, flags,
                                           worker_scratch_->match_cb_user_data_);
        if (cease) {
          break;
        }
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
        cease = worker_scratch_->match_cb_(real_id, from, to, flags,
                                           worker_scratch_->match_cb_user_data_);
      }
    }
  } else {
    cease =
        worker_scratch_->match_cb_(real_id, from, to, flags, worker_scratch_->match_cb_user_data_);
  }

  return cease;
}
} // namespace Hyperscan
} // namespace Common
} // namespace SrSecurity