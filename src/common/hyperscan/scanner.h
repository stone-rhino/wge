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