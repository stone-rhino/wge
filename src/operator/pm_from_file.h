#pragma once

#include <fstream>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "operator_base.h"

#include "../common/hyperscan/scanner.h"
#include "../common/log.h"

namespace SrSecurity {
namespace Operator {
/**
 * Performs a case-insensitive match of the provided phrases against the desired input value. The
 * operator uses a set-based matching algorithm (Aho-Corasick), which means that it will match any
 * number of keywords in parallel. When matching of a large number of keywords is needed, this
 * operator performs much better than a regular expression.
 */
class PmFromFile : public OperatorBase {
  DECLARE_OPERATOR_NAME(pmFromFile);

public:
  PmFromFile(std::string&& literal_value, bool is_not)
      : OperatorBase(std::move(literal_value), is_not) {
    // Load the hyperscan database.
    // We cache the hyperscan database to avoid loading(complie) the same database multiple times.
    auto iter = database_cache_.find(literal_value_);
    if (iter == database_cache_.end()) {
      std::ifstream ifs(literal_value_);
      if (!ifs.is_open()) {
        SRSECURITY_LOG_ERROR("Failed to open hyperscan database file: {}", literal_value_);
        return;
      }

      auto hs_db = std::make_shared<Common::Hyperscan::HsDataBase>(ifs, true, false);
      database_ = hs_db;
      database_cache_.emplace(literal_value_, hs_db);
    } else {
      database_ = iter->second;
    }
  }

  PmFromFile(const std::shared_ptr<Macro::MacroBase> macro, bool is_not)
      : OperatorBase(macro, is_not) {
    // Not supported macro expansion
    UNREACHABLE();
  }

public:
  bool evaluate(Transaction& t, const Common::Variant& operand) const override {
    if (!database_) [[unlikely]] {
      return false;
    }

    if (!IS_STRING_VIEW_VARIANT(operand)) [[unlikely]] {
      return false;
    }

    // The Common::Hyperscan::Scanner::blockScan() method is stateful, so we need to create a new
    // scanner for each evaluation. Luckily, the hyperscan database is cached, so the cost of
    // creating a new scanner is minimal.
    Common::Hyperscan::Scanner scanner(database_);
    bool matched = false;
    scanner.registMatchCallback(
        [](uint64_t id, unsigned long long from, unsigned long long to, unsigned int flags,
           void* user_data) -> int {
          bool* matched = static_cast<bool*>(user_data);
          *matched = true;
          return 1;
        },
        &matched);

    scanner.blockScan(std::get<std::string_view>(operand));

    return is_not_ ^ matched;
  }

private:
  std::shared_ptr<Common::Hyperscan::HsDataBase> database_;

  // Cache the hyperscan database
  static std::unordered_map<std::string, std::shared_ptr<Common::Hyperscan::HsDataBase>>
      database_cache_;
};
} // namespace Operator
} // namespace SrSecurity