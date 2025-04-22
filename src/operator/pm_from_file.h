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

#include <fstream>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "operator_base.h"

#include "../common/evaluate_result.h"
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

      auto hs_db = std::make_shared<Common::Hyperscan::HsDataBase>(ifs, true, true);
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
    std::pair<unsigned long long, unsigned long long> result(0, 0);
    scanner_->registMatchCallback(
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
    scanner_->blockScan(operand_str);

    bool matched = result.first != result.second;
    if (matched) {
      Common::EvaluateResults::Element value;
      value.variant_ =
          std::string_view(operand_str.data() + result.first, result.second - result.first);
      t.addCapture(std::move(value));
    }

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