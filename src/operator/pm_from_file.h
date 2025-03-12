#pragma once

#include <fstream>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "operator_base.h"

#include "../common/file.h"
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
  PmFromFile(std::string&& literal_value, bool is_not, std::string_view curr_rule_file_path)
      : OperatorBase(std::move(literal_value), is_not) {
    // Make the file path absolute.
    std::string file_path = Common::File::makeFilePath(curr_rule_file_path, literal_value_);

    // Load the hyperscan database and create a scanner.
    // We cache the hyperscan database to avoid loading(complie) the same database multiple times.
    auto iter = database_cache_.find(file_path);
    if (iter == database_cache_.end()) {
      std::ifstream ifs(file_path);
      if (!ifs.is_open()) {
        SRSECURITY_LOG_ERROR("Failed to open hyperscan database file: {}", file_path);
        return;
      }

      auto hs_db = std::make_shared<Common::Hyperscan::HsDataBase>(ifs, true, false);
      scanner_ = std::make_unique<Common::Hyperscan::Scanner>(hs_db);
      database_cache_.emplace(file_path, hs_db);
    } else {
      scanner_ = std::make_unique<Common::Hyperscan::Scanner>(iter->second);
    }
  }

  PmFromFile(const std::shared_ptr<Macro::MacroBase> macro, bool is_not,
             std::string_view curr_rule_file_path)
      : OperatorBase(macro, is_not) {
    // Not supported macro expansion
    UNREACHABLE();
  }

public:
  bool evaluate(Transaction& t, const Common::Variant& operand) const override {
    if (!scanner_) [[unlikely]] {
      return false;
    }

    if (!IS_STRING_VIEW_VARIANT(operand)) [[unlikely]] {
      return false;
    }

    // The hyperscan scanner is thread-safe, so we can use the same scanner for all transactions.
    // Actually, the scanner uses a thread-local scratch space to avoid the overhead of creating a
    // scratch space for each transaction.
    bool matched = false;
    scanner_->registMatchCallback(
        [](uint64_t id, unsigned long long from, unsigned long long to, unsigned int flags,
           void* user_data) -> int {
          bool* matched = static_cast<bool*>(user_data);
          *matched = true;
          return 1;
        },
        &matched);
    scanner_->blockScan(std::get<std::string_view>(operand));

    return is_not_ ^ matched;
  }

private:
  std::unique_ptr<Common::Hyperscan::Scanner> scanner_;

  // Cache the hyperscan database
  static std::unordered_map<std::string, std::shared_ptr<Common::Hyperscan::HsDataBase>>
      database_cache_;
};
} // namespace Operator
} // namespace SrSecurity