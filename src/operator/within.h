#pragma once

#include "operator_base.h"

#include "../common/hyperscan/scanner.h"

namespace SrSecurity {
namespace Operator {
/**
 * Returns true if the input value (the needle) is found anywhere within the @within parameter (the
 * haystack). Macro expansion is performed on the parameter string before comparison.
 */
class Within : public OperatorBase {
  DECLARE_OPERATOR_NAME(within);

public:
  Within(std::string&& literal_value, bool is_not, std::string_view curr_rule_file_path)
      : OperatorBase(std::move(literal_value), is_not) {
    // Split the literal value into tokens.
    std::vector<std::string_view> tokens = SplitTokens(literal_value_);

    // Calculate the order independent hash value of all tokens.
    size_t hash = calcOrderIndependentHash(tokens);

    // Load the hyperscan database and create a scanner.
    // We cache the hyperscan database to avoid loading(complie) the same database multiple times.
    auto iter = database_cache_.find(hash);
    if (iter == database_cache_.end()) {
      auto hs_db = std::make_shared<Common::Hyperscan::HsDataBase>(tokens, true, false);
      scanner_ = std::make_unique<Common::Hyperscan::Scanner>(hs_db);
      database_cache_.emplace(hash, hs_db);
    } else {
      scanner_ = std::make_unique<Common::Hyperscan::Scanner>(iter->second);
    }
  }

  Within(const std::shared_ptr<Macro::MacroBase> macro, bool is_not,
         std::string_view curr_rule_file_path)
      : OperatorBase(macro, is_not) {}

public:
  bool evaluate(Transaction& t, const Common::Variant& operand) const override {
    if (!IS_STRING_VIEW_VARIANT(operand)) [[unlikely]] {
      return false;
    }

    Common::Hyperscan::Scanner* scanner = scanner_.get();
    std::unique_ptr<Common::Hyperscan::Scanner> macro_scanner;

    // If there is a macro, expand it and create or reuse a scanner.
    if (macro_) [[unlikely]] {
      MACRO_EXPAND_STRING_VIEW(macro_value);

      // Split the literal value into tokens.
      std::vector<std::string_view> tokens = SplitTokens(macro_value);

      // Calculate the order independent hash value of all tokens.
      size_t hash = calcOrderIndependentHash(tokens);

      // All the threads will try to access the database_cache_ at the same time, so we need to
      // lock the database_cache_mutex_.
      // May be we can use thread local storage to store the database, to avoid the lock. But the
      // probablity of the macro expansion is very low, so we use the lock here.
      std::lock_guard<std::mutex> lock(database_cache_mutex_);

      auto iter = database_cache_.find(hash);
      if (iter == database_cache_.end()) {
        auto hs_db = std::make_shared<Common::Hyperscan::HsDataBase>(tokens, true, false);
        macro_scanner = std::make_unique<Common::Hyperscan::Scanner>(hs_db);
        scanner = macro_scanner.get();
        database_cache_.emplace(hash, hs_db);
      } else {
        macro_scanner = std::make_unique<Common::Hyperscan::Scanner>(iter->second);
        scanner = macro_scanner.get();
      }
    }

    // The hyperscan scanner is thread-safe, so we can use the same scanner for all transactions.
    // Actually, the scanner uses a thread-local scratch space to avoid the overhead of creating a
    // scratch space for each transaction.
    bool matched = false;
    scanner->registMatchCallback(
        [](uint64_t id, unsigned long long from, unsigned long long to, unsigned int flags,
           void* user_data) -> int {
          bool* matched = static_cast<bool*>(user_data);
          *matched = true;
          return 1;
        },
        &matched);
    scanner->blockScan(std::get<std::string_view>(operand));

    return is_not_ ^ matched;
  }

private:
  std::vector<std::string_view> SplitTokens(std::string_view value) const {
    // Split by space
    constexpr char delimiter = ' ';

    std::vector<std::string_view> tokens;
    size_t pos = 0;
    size_t next_pos = 0;

    while ((next_pos = value.find(delimiter, pos)) != std::string_view::npos) {
      tokens.emplace_back(value.substr(pos, next_pos - pos));
      pos = next_pos + 1;
    }

    if (pos < value.size()) {
      tokens.emplace_back(value.substr(pos));
    }

    return tokens;
  }

  size_t calcOrderIndependentHash(const std::vector<std::string_view>& tokens) const {
    size_t hash = 0;
    std::vector<size_t> token_hashes;
    for (auto token : tokens) {
      token_hashes.emplace_back(std::hash<std::string_view>{}(token));
    }
    std::sort(token_hashes.begin(), token_hashes.end());
    for (auto token_hash : token_hashes) {
      hash = hash * 31 + token_hash;
    }
    return hash;
  }

private:
  std::unique_ptr<Common::Hyperscan::Scanner> scanner_;

  // Cache the hyperscan database
  // key: order independent hash value of all tokens
  // value: hyperscan database
  static std::unordered_map<size_t, std::shared_ptr<Common::Hyperscan::HsDataBase>> database_cache_;
  static std::mutex database_cache_mutex_;
};
} // namespace Operator
} // namespace SrSecurity