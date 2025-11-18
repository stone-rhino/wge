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

#include "operator_base.h"

#include "../common/evaluate_result.h"
#include "../common/hyperscan/scanner.h"
#include "../common/lru_cache.hpp"
#include "../common/string.h"

namespace Wge {
namespace Operator {
/**
 * Returns true if the input value (the needle) is found anywhere within the @within parameter (the
 * haystack). Macro expansion is performed on the parameter string before comparison.
 */
class Within final : public OperatorBase {
  DECLARE_OPERATOR_NAME(within);

public:
  Within(std::string&& literal_value, bool is_not, std::string_view curr_rule_file_path)
      : OperatorBase(std::move(literal_value), is_not) {
    // Split the literal value into tokens.
    std::vector<std::string_view> tokens = Common::SplitTokens(literal_value_);

    // Calculate the order independent hash value of all tokens.
    int64_t hash = calcOrderIndependentHash(tokens);

    // Load the hyperscan database and create a scanner.
    // We cache the hyperscan database to avoid loading(complie) the same database multiple times.
    database_cache_.access(
        hash,
        [&](const std::shared_ptr<Common::Hyperscan::HsDataBase>& hs_db) {
          scanner_ = std::make_unique<Common::Hyperscan::Scanner>(hs_db);
        },
        [&]() {
          auto hs_db = std::make_shared<Common::Hyperscan::HsDataBase>(tokens, true, true, true,
                                                                       false, false);
          scanner_ = std::make_unique<Common::Hyperscan::Scanner>(hs_db);
          return hs_db;
        });
  }

  Within(std::unique_ptr<Macro::MacroBase>&& macro, bool is_not,
         std::string_view curr_rule_file_path)
      : OperatorBase(std::move(macro), is_not) {}

public:
  bool evaluate(Transaction& t, const Common::Variant& operand) const override {
    if (!IS_STRING_VIEW_VARIANT(operand))
      [[unlikely]] { return false; }

    std::unique_ptr<Common::Hyperscan::Scanner> macro_scanner;
    Common::Hyperscan::Scanner* scanner = scanner_.get();

    // If there is a macro, expand it and create or reuse a scanner.
    if (macro_)
      [[unlikely]] {
        MACRO_EXPAND_STRING_VIEW(macro_value);

        // Split the literal value into tokens.
        std::vector<std::string_view> tokens = Common::SplitTokens(macro_value);

        // Calculate the order independent hash value of all tokens.
        int64_t hash = calcOrderIndependentHash(tokens);

        // Load the hyperscan database and create a scanner.
        // We cache the hyperscan database to avoid loading(complie) the same database multiple
        // times.
        database_cache_.access(
            hash,
            [&](const std::shared_ptr<Common::Hyperscan::HsDataBase>& hs_db) {
              macro_scanner = std::make_unique<Common::Hyperscan::Scanner>(hs_db);
              scanner = macro_scanner.get();
            },
            [&]() {
              return std::make_shared<Common::Hyperscan::HsDataBase>(tokens, true, true, true,
                                                                     false, false);
            });
      }

    // The hyperscan scanner is thread-safe, so we can use the same scanner for all transactions.
    // Actually, the scanner uses a thread-local scratch space to avoid the overhead of creating a
    // scratch space for each transaction.
    std::pair<unsigned long long, unsigned long long> result(0, 0);
    scanner->registMatchCallback(
        [](uint64_t id, unsigned long long from, unsigned long long to, unsigned int flags,
           void* user_data) -> int {
          std::pair<unsigned long long, unsigned long long>* result =
              static_cast<std::pair<unsigned long long, unsigned long long>*>(user_data);
          result->first = from;
          result->second = to;
          return 1;
        },
        &result);
    std::string_view operand_str = std::get<std::string_view>(operand);
    scanner->blockScan(std::get<std::string_view>(operand));

    bool matched = result.first != result.second;
    if (matched) {
      t.stageCapture(
          0, t.internString({operand_str.data() + result.first, result.second - result.first}));
    }

    return matched;
  }

private:
  int64_t calcOrderIndependentHash(const std::vector<std::string_view>& tokens) const {
    int64_t hash = 0;
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
  static Common::LruCache<int64_t, std::shared_ptr<Common::Hyperscan::HsDataBase>, 101>
      database_cache_;
};
} // namespace Operator
} // namespace Wge