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

namespace SrSecurity {
namespace Common {
namespace Hyperscan {
class Scanner {
public:
  Scanner(const std::shared_ptr<HsDataBase> hs_db);

public:
  void registMatchCallback(Scratch::MatchCallback cb, void* user_data) const;
  void registPcreRemoveDuplicateCallback(Scratch::PcreRemoveDuplicateCallbak cb,
                                         void* user_data) const;

public:
  void blockScan(std::string_view data) const;
  void streamScanStart() const;
  void streamScan(std::string_view data) const;
  void streamScanStop() const;

private:
  static int matchCallback(unsigned int id, unsigned long long from, unsigned long long to,
                           unsigned int flags, void* user_data);

private:
  static thread_local std::unique_ptr<Scratch> worker_scratch_;
  const std::shared_ptr<HsDataBase> hs_db_;
  std::unique_ptr<Pcre::Scanner> pcre_;
};
} // namespace Hyperscan
} // namespace Common
} // namespace SrSecurity