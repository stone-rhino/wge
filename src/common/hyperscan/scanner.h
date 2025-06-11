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
#include <memory>
#include <string_view>

#include "hs_database.h"

#include "../pcre/scanner.h"

namespace Wge {
namespace Common {
namespace Hyperscan {
class Scanner {
public:
  Scanner(const std::shared_ptr<HsDataBase> hs_db);

public:
  void registMatchCallback(Scratch::MatchCallback cb, void* user_data) const;
  void registPcreRemoveDuplicateCallback(Scratch::PcreRemoveDuplicateCallbak cb,
                                         void* user_data) const;
  void setMaxPcreScanFrontLen(unsigned long long len) { max_pcre_scan_front_len_ = len; }
  void setMaxPcreScanBackLen(unsigned long long len) { max_pcre_scan_back_len_ = len; }
  void setPcreMatchLimit(size_t match_limit) const { pcre_->setMatchLimit(match_limit); }

public:
  enum class ScanMode {
    Normal,          // Normal scan
    GreedyAndGlobal, // Greedy and global scan
    Greedy           // Greedy scan
  };

  void blockScan(std::string_view data, ScanMode mode = ScanMode::Normal,
                 Scratch::MatchCallback cb = nullptr, void* user_data = nullptr) const;
  void streamScanStart() const;
  void streamScan(std::string_view data) const;
  void streamScanStop() const;
  const std::string& databaseSha1() const { return hs_db_->sha1(); }

private:
  using GreedyMatchCache = std::unordered_map<unsigned int,                          // id
                                              std::unordered_map<unsigned long long, // from
                                                                 unsigned long long  // to
                                                                 >>;

private:
  static int matchCallback(unsigned int id, unsigned long long from, unsigned long long to,
                           unsigned int flags, void* user_data);
  static int greedyMatchCallback(unsigned int id, unsigned long long from, unsigned long long to,
                                 unsigned int flags, void* user_data);

private:
  static thread_local std::unique_ptr<Scratch> worker_scratch_;
  const std::shared_ptr<HsDataBase> hs_db_;
  std::unique_ptr<Pcre::Scanner> pcre_;
  unsigned long long max_pcre_scan_front_len_{std::numeric_limits<unsigned int>::max()};
  unsigned long long max_pcre_scan_back_len_{std::numeric_limits<unsigned int>::max()};
};
} // namespace Hyperscan
} // namespace Common
} // namespace Wge