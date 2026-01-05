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

#include <string_view>
#include <unordered_set>
#include <variant>

#include "variable_base.h"

#include "../common/file.h"
#include "../common/hyperscan/scanner.h"
#include "../common/literal_match/scanner.h"
#include "../common/log.h"
#include "../common/pcre/scanner.h"
#include "../common/re2/scanner.h"
#include "../rule.h"
#include "../transaction.h"

namespace Wge {
namespace Variable {
/**
 * Base class for collection variables.
 */
class CollectionBase : public VariableBase {
public:
  CollectionBase(std::string&& sub_name, bool is_not, bool is_counter,
                 std::string_view curr_rule_file_path)
      : VariableBase(std::move(sub_name), is_not, is_counter),
        curr_rule_file_path_(curr_rule_file_path) {
    if (sub_name_.size() >= 3) {
      bool hyperscan = false;
      if (sub_name_.front() == '/' && sub_name_.back() == '/') {
        regex_accept_scanner_ =
            createScanner(std::string_view(sub_name_.data() + 1, sub_name_.size() - 2), false);
      } else if (sub_name_.front() == '@' && sub_name_.back() == '@') {
        regex_accept_scanner_ =
            createScanner(std::string_view(sub_name_.data() + 1, sub_name_.size() - 2), true);
      }
    }
  }
  virtual ~CollectionBase() = default;

public:
  bool isCollection() const override { return sub_name_.empty() ? true : isRegex(); };

public:
  /**
   * Add a variable to the exception list.
   * @param variable_sub_name the sub name of the variable.
   */
  void addExceptVariable(std::string_view variable_sub_name) {
    if (variable_sub_name.front() == '/' && variable_sub_name.back() == '/') {
      regex_except_scanners_.emplace_back(createScanner(
          std::string_view(variable_sub_name.data() + 1, variable_sub_name.size() - 2), false));
    } else if (variable_sub_name.front() == '@' && variable_sub_name.back() == '@') {
      regex_except_scanners_.emplace_back(createScanner(
          std::string_view(variable_sub_name.data() + 1, variable_sub_name.size() - 2), true));
    } else {
      except_variables_.insert(variable_sub_name);
    }
  }

  /**
   * Check whether the variable is in the exception list.
   * @param t the transaction.
   * @param variable_main_name the main name of the variable.
   * @param variable_sub_name the sub name of the variable.
   * @return true if the variable is in the exception list, false otherwise.
   */
  bool hasExceptVariable(Transaction& t, std::string_view variable_main_name,
                         std::string_view variable_sub_name) const {
    if (!except_variables_.empty())
      [[unlikely]] {
        if (except_variables_.find(variable_sub_name) != except_variables_.end())
          [[unlikely]] {
            WGE_LOG_TRACE("variable {}:{} is in the except list", variable_main_name,
                          variable_sub_name);
            return true;
          }
      }

    bool match = false;
    if (!regex_except_scanners_.empty())
      [[unlikely]] {
        for (auto& except_scanner : regex_except_scanners_) {
          std::visit(
              [&](auto&& scanner) {
                using T = std::decay_t<decltype(scanner)>;
                if constexpr (std::is_same_v<T, std::monostate>) {
                  // No scanner, do nothing
                } else if constexpr (std::is_same_v<T,
                                                    std::unique_ptr<Common::Hyperscan::Scanner>>) {
                  scanner->blockScan(
                      variable_sub_name, Common::Hyperscan::Scanner::ScanMode::Normal,
                      [](uint64_t id, unsigned long long from, unsigned long long to,
                         unsigned int flags, void* user_data) {
                        bool* match = reinterpret_cast<bool*>(user_data);
                        *match = true;
                        return 1;
                      },
                      &match);
                  if (match) {
                    WGE_LOG_TRACE("variable {}:{} is in the except list by pmf", variable_main_name,
                                  variable_sub_name);
                  }
                } else {
                  match = scanner->match(variable_sub_name);
                  if (match) {
                    WGE_LOG_TRACE("variable {}:{} is in the except list by regex",
                                  variable_main_name, variable_sub_name);
                  }
                }
              },
              except_scanner);
          if (match) {
            return true;
          }
        }
      }

    // Check if the variable is removed by the ctl action
    const Rule* rule = t.getCurrentEvaluateRule();
    // Only top-level rules can remove variables by ctl action
    if (rule && rule->chainIndex() == -1) {
      Variable::FullName full_name{variable_main_name, variable_sub_name};
      if (t.isRuleTargetRemoved(rule, full_name)) {
        WGE_LOG_TRACE("variable {}:{} is removed by ctl action", variable_main_name,
                      variable_sub_name);
        return true;
      }
    }

    return false;
  }

  bool isRegex() const { return !std::holds_alternative<std::monostate>(regex_accept_scanner_); }

  bool match(std::string_view subject) const {
    bool match = false;
    std::visit(
        [&](auto&& scanner) {
          using T = std::decay_t<decltype(scanner)>;
          if constexpr (std::is_same_v<T, std::monostate>) {
            // No scanner, do nothing
          } else if constexpr (std::is_same_v<T, std::unique_ptr<Common::Hyperscan::Scanner>>) {
            scanner->blockScan(
                subject, Common::Hyperscan::Scanner::ScanMode::Normal,
                [](uint64_t id, unsigned long long from, unsigned long long to, unsigned int flags,
                   void* user_data) {
                  bool* match = reinterpret_cast<bool*>(user_data);
                  *match = true;
                  return 1;
                },
                &match);
          } else {
            match = scanner->match(subject);
          }
        },
        regex_accept_scanner_);

    return match;
  }

protected:
  std::unordered_set<std::string_view> except_variables_;

private:
  using Scanner = std::variant<
      std::monostate, std::unique_ptr<Common::Re2::Scanner>, std::unique_ptr<Common::Pcre::Scanner>,
      std::unique_ptr<Common::LiteralMatch::Scanner>, std::unique_ptr<Common::Hyperscan::Scanner>>;

private:
  Scanner createScanner(std::string_view pattern, bool hyperscan) const {
    Scanner scanner;
    if (hyperscan) {
      // Make the file path absolute.
      std::string temp_file_path(pattern.data(), pattern.size());
      std::string file_path = Common::File::makeFilePath(curr_rule_file_path_, temp_file_path);

      // Load the hyperscan database and create a scanner.
      // We cache the hyperscan database to avoid loading(complie) the same database multiple
      // times.
      auto iter = database_cache_.find(file_path);
      if (iter == database_cache_.end()) {
        std::ifstream ifs(file_path);
        if (!ifs.is_open()) {
          WGE_LOG_ERROR("Failed to open hyperscan database file: {}", file_path);
          return scanner;
        }

        auto hs_db =
            std::make_shared<Common::Hyperscan::HsDataBase>(ifs, false, true, false, false, false);
        scanner = std::make_unique<Common::Hyperscan::Scanner>(hs_db);
        database_cache_.emplace(file_path, hs_db);
      } else {
        scanner = std::make_unique<Common::Hyperscan::Scanner>(iter->second);
      }
      return scanner;
    }

    if (Common::LiteralMatch::Scanner::isLiteralPattern(pattern)) {
      scanner = std::make_unique<Common::LiteralMatch::Scanner>(pattern, false);
    } else {
      auto re2 = std::make_unique<Common::Re2::Scanner>(pattern, false, false);
      if (re2->ok()) {
        scanner = std::move(re2);
      } else {
        scanner = std::make_unique<Common::Pcre::Scanner>(pattern, false, false);
      }
    }
    return scanner;
  }

private:
  Scanner regex_accept_scanner_;
  std::vector<Scanner> regex_except_scanners_;
  std::string_view curr_rule_file_path_;
  // Cache the hyperscan database
  static std::unordered_map<std::string, std::shared_ptr<Common::Hyperscan::HsDataBase>>
      database_cache_;
}; // namespace Variable
} // namespace Variable
} // namespace Wge