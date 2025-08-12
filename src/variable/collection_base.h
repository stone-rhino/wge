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

#include "../common/file.h"
#include "../common/hyperscan/scanner.h"
#include "../common/log.h"
#include "../common/pcre/scanner.h"
#include "../rule.h"
#include "../transaction.h"

namespace Wge {
namespace Variable {
/**
 * Base class for collection variables.
 */
class CollectionBase {
public:
  CollectionBase(const std::string& sub_name, std::string_view curr_rule_file_path)
      : curr_rule_file_path_(curr_rule_file_path) {
    if (sub_name.size() >= 3) {
      if (sub_name.front() == '/' && sub_name.back() == '/') {
        regex_accept_scanner_ = std::make_unique<Common::Pcre::Scanner>(
            std::string_view{sub_name.data() + 1, sub_name.size() - 2}, false, false);
      } else if (sub_name.front() == '@' && sub_name.back() == '@') {
        // Make the file path absolute.
        std::string temp_file_path(sub_name.data() + 1, sub_name.size() - 2);
        std::string file_path = Common::File::makeFilePath(curr_rule_file_path_, temp_file_path);

        // Load the hyperscan database and create a scanner.
        // We cache the hyperscan database to avoid loading(complie) the same database multiple
        // times.
        auto iter = database_cache_.find(file_path);
        if (iter == database_cache_.end()) {
          std::ifstream ifs(file_path);
          if (!ifs.is_open()) {
            WGE_LOG_ERROR("Failed to open hyperscan database file: {}", file_path);
            return;
          }

          auto hs_db =
              std::make_shared<Common::Hyperscan::HsDataBase>(ifs, true, true, false, false, false);
          pmf_accept_scanner_ = std::make_unique<Common::Hyperscan::Scanner>(hs_db);
          database_cache_.emplace(file_path, hs_db);
        } else {
          pmf_accept_scanner_ = std::make_unique<Common::Hyperscan::Scanner>(iter->second);
        }
      }
    }
  }
  virtual ~CollectionBase() = default;

public:
  /**
   * Add a variable to the exception list.
   * @param variable_sub_name the sub name of the variable.
   */
  void addExceptVariable(std::string_view variable_sub_name) {
    if (variable_sub_name.front() == '/' && variable_sub_name.back() == '/') {
      regex_except_scanners_.emplace_back(std::make_unique<Common::Pcre::Scanner>(
          std::string_view{variable_sub_name.data() + 1, variable_sub_name.size() - 2}, false,
          false));
    } else if (variable_sub_name.front() == '@' && variable_sub_name.back() == '@') {
      // Make the file path absolute.
      std::string temp_file_path(variable_sub_name.data() + 1, variable_sub_name.size() - 2);
      std::string file_path = Common::File::makeFilePath(curr_rule_file_path_, temp_file_path);

      // Load the hyperscan database and create a scanner.
      // We cache the hyperscan database to avoid loading(complie) the same database multiple
      // times.
      auto iter = database_cache_.find(file_path);
      if (iter == database_cache_.end()) {
        std::ifstream ifs(file_path);
        if (!ifs.is_open()) {
          WGE_LOG_ERROR("Failed to open hyperscan database file: {}", file_path);
          return;
        }

        auto hs_db =
            std::make_shared<Common::Hyperscan::HsDataBase>(ifs, true, true, false, false, false);
        pmf_except_scanners_.emplace_back(std::make_unique<Common::Hyperscan::Scanner>(hs_db));
        database_cache_.emplace(file_path, hs_db);
      } else {
        pmf_except_scanners_.emplace_back(
            std::make_unique<Common::Hyperscan::Scanner>(iter->second));
      }
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
    if (except_variables_.find(variable_sub_name) != except_variables_.end()) {
      WGE_LOG_TRACE("variable {}:{} is in the except list", variable_main_name, variable_sub_name);
      return true;
    }

    for (auto& except_scanner : regex_except_scanners_) {
      if (except_scanner->match(variable_sub_name)) {
        WGE_LOG_TRACE("variable {}:{} is in the except list by regex", variable_main_name,
                      variable_sub_name);
        return true;
      }
    }

    for (auto& except_scanner : pmf_except_scanners_) {
      bool match = false;
      pmf_accept_scanner_->blockScan(
          variable_sub_name, Common::Hyperscan::Scanner::ScanMode::Normal,
          [](uint64_t id, unsigned long long from, unsigned long long to, unsigned int flags,
             void* user_data) {
            bool* match = reinterpret_cast<bool*>(user_data);
            *match = true;
            return 1;
          },
          &match);
      if (match) {
        WGE_LOG_TRACE("variable {}:{} is in the except list by pmf", variable_main_name,
                      variable_sub_name);
        return true;
      }
    }

    // Check if the variable is removed by the ctl action
    Variable::FullName full_name{variable_main_name, variable_sub_name};
    const Rule* rule = t.getCurrentEvaluateRule();
    // Only top-level rules can remove variables by ctl action
    if (rule && rule->chainIndex() == -1) {
      if (t.isRuleTargetRemoved(rule, full_name)) {
        WGE_LOG_TRACE("variable {}:{} is removed by ctl action", variable_main_name,
                      variable_sub_name);
        return true;
      }
    }

    return false;
  }

  bool isRegex() const {
    return regex_accept_scanner_ != nullptr || pmf_accept_scanner_ != nullptr;
  }

  bool match(std::string_view subject) const {
    bool match = false;
    if (regex_accept_scanner_) {
      match = regex_accept_scanner_->match(subject);
    } else if (pmf_accept_scanner_) {
      pmf_accept_scanner_->blockScan(
          subject, Common::Hyperscan::Scanner::ScanMode::Normal,
          [](uint64_t id, unsigned long long from, unsigned long long to, unsigned int flags,
             void* user_data) {
            bool* match = reinterpret_cast<bool*>(user_data);
            *match = true;
            return 1;
          },
          &match);
    }

    return match;
  }

protected:
  std::unordered_set<std::string_view> except_variables_;

private:
  std::unique_ptr<Common::Pcre::Scanner> regex_accept_scanner_;
  std::vector<std::unique_ptr<Common::Pcre::Scanner>> regex_except_scanners_;
  std::unique_ptr<Common::Hyperscan::Scanner> pmf_accept_scanner_;
  std::vector<std::unique_ptr<Common::Hyperscan::Scanner>> pmf_except_scanners_;
  std::string_view curr_rule_file_path_;
  // Cache the hyperscan database
  static std::unordered_map<std::string, std::shared_ptr<Common::Hyperscan::HsDataBase>>
      database_cache_;
}; // namespace Variable
} // namespace Variable
} // namespace Wge