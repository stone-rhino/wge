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
#include <mutex>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "operator_base.h"

#include "../common/evaluate_result.h"
#include "../common/file.h"
#include "../common/hyperscan/scanner.h"
#include "../common/log.h"

namespace Wge {
namespace Operator {
/**
 * Performs a case-insensitive match of the provided phrases against the desired input value. The
 * operator uses a set-based matching algorithm (Aho-Corasick), which means that it will match any
 * number of keywords in parallel. When matching of a large number of keywords is needed, this
 * operator performs much better than a regular expression.
 *
 * The file format is as follows:
 * - Each line represents a separate pattern.
 * - Lines starting with "##!^ " are treated as prefixes. The patterns that follow will be prefixed
 * with this value, but the previous patterns will not be affected.
 * - Lines starting with "##!$ " are treated as suffixes. The patterns that follow will be suffixed
 * with this value, but the previous patterns will not be affected.
 * - Lines starting with "##!+ i" enable case-insensitive matching. The patterns that follow will be
 * case-insensitive, but the previous patterns will not be affected. Default is enabled.
 * - Lines starting with "##!+ -i" disable case-insensitive matching. The patterns that follow will
 * be case-sensitive, but the previous patterns will not be affected.
 * - Lines starting with "##!+ l" enable literal matching. The all patterns in the file will be
 * affected. Default is enable.
 * - Lines starting with "##!+ -l" disable literal matching. The all patterns in the file will be
 * affected.
 */
class PmFromFile final : public OperatorBase {
  DECLARE_OPERATOR_NAME(pmFromFile);

public:
  PmFromFile(std::string&& literal_value, bool is_not, std::string_view curr_rule_file_path)
      : OperatorBase(std::move(literal_value), is_not), curr_rule_file_path_(curr_rule_file_path),
        expression_list_(true) {
    // Make the file path absolute.
    std::string file_path = Common::File::makeFilePath(curr_rule_file_path, literal_value_);

    // Load the expression list
    std::ifstream ifs(file_path);
    if (ifs.is_open()) {
      expression_list_.load(ifs, true, true, true, false, false);
    } else {
      WGE_LOG_ERROR("Failed to open hyperscan database file: {}", file_path);
    }
  }

  PmFromFile(std::unique_ptr<Macro::MacroBase>&& macro, bool is_not,
             std::string_view curr_rule_file_path)
      : OperatorBase(std::move(macro), is_not), expression_list_(true) {
    // Not supported macro expansion
    UNREACHABLE();
  }

public:
  // The PmFromFile operator must be called init before call the evaluate function
  void init(const std::string& serialize_dir);

public:
  bool evaluate(Transaction& t, const Common::Variant& operand) const override {
    if (!scanner_)
      [[unlikely]] { return false; }

    if (!IS_STRING_VIEW_VARIANT(operand))
      [[unlikely]] { return false; }

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
      t.stageCapture(0, {operand_str.data() + result.first, result.second - result.first});
    }

    return matched;
  }

private:
  std::unique_ptr<Common::Hyperscan::Scanner> scanner_;

  // Cache the hyperscan database
  static std::unordered_map<std::string, std::shared_ptr<Common::Hyperscan::HsDataBase>>
      database_cache_;
  static std::mutex database_cache_mutex_;

  std::string_view curr_rule_file_path_;
  Common::Hyperscan::ExpressionList expression_list_;
};
} // namespace Operator
} // namespace Wge