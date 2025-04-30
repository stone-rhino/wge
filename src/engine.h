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

#include <expected>
#include <memory>
#include <optional>
#include <unordered_map>
#include <vector>

#include "common/log.h"
#include "marker.h"
#include "persistent_storage/storage.h"
#include "rule.h"
#include "transaction.h"

namespace Wge::Antlr4 {
class Parser;
}

namespace Wge {

/**
 * The engine is the core of the WAF.
 * It is responsible for loading the rule set, parsing the rule set, and make a transaction to
 * evaluate the rules. The engine is a singleton, and only one instance of the engine exists in the
 * life of the program.
 */
class Engine final {
public:
  /**
   * Construct the engine
   * @param level the debug log level. if the WGE_LOG_ACTIVE_LEVEL compile-time macro is not
   * defined, the debug log will be disabled. and the log level will be ignored.
   * @param log_file the log file path. If it is empty, the log will be output to the console
   */
  Engine(spdlog::level::level_enum level = spdlog::level::info, const std::string& log_file = "");

public:
  /**
   * Load the rule set from a file
   * @param file_path the rule(SecLang) file path. support absolute path and relative path
   * @result an error string is returned if fails, and returned true otherwise
   */
  std::expected<bool, std::string> loadFromFile(const std::string& file_path);

  /**
   * Load the rule set from a configuration directive
   * @param directive configuration directive. such as "SecRuleEngine On"
   * @result an error string is returned if fails, and returned true otherwise
   */
  std::expected<bool, std::string> load(const std::string& directive);

  /**
   * Initialize the engine
   * @note must call once before call makeTransaction, and only once in the life of the engine
   * instance.
   */
  void init();

  /**
   * Get default actions
   * @param phase specify the phase of the default actions, the valid range is 1-5.
   * @return vector of default actions
   */
  const Rule* defaultActions(int phase) const;

  /**
   * Get rules
   * @param phase specify the phase of rule, the valid range is 1-5.
   * @return vector of rules
   */
  const std::vector<const Rule*>& rules(int phase) const;

public:
  /**
   * Make a transaction to evaluate rules.
   * @return pointer of transaction
   * @note must call init before call this method
   */
  TransactionPtr makeTransaction() const;

  /**
   * Get the engine configuration
   * @return reference of engine configuration
   */
  const EngineConfig& config() const;

  /**
   * Get the audit log configuration
   * @return reference of audit log configuration
   */
  const AuditLogConfig& auditLogConfig() const;

  /**
   * Find the rule by id
   * @param id the rule id
   * @return pointer of rule if found, and nullptr otherwise
   */
  const Rule* findRuleById(uint64_t id) const;

  /**
   * Find the rule by tag
   * @param tag the rule tag
   */
  void findRuleByTag(const std::string& tag,
                     std::array<std::unordered_set<const Rule*>, PHASE_TOTAL>& rule_set) const;

  /**
   * Get the transaction variable index
   * @param name the variable name
   * @param force if true, insert a new index if the variable is not found
   * @return the index of the variable if found, and std::nullopt otherwise
   */
  std::optional<size_t> getTxVariableIndex(const std::string& name) const;

  /**
   * Get the transaction variable index reverse
   * @param index the index of the variable
   * @return the variable name. if the index is out of range, an empty string is returned
   */
  std::string_view getTxVariableIndexReverse(size_t index) const;

  std::optional<const std::vector<const Rule*>::iterator> marker(const std::string& name,
                                                                 int phase) const;

  /**
   * Get persistent storage
   * @return reference of persistent storage
   */
  PersistentStorage::Storage& storage() const { return storage_; }

private:
  void initDefaultActions();
  void initRules();
  void initMakers();

private:
  // Is the engine initialized
  bool is_init_{false};

  // The parser is used to parse the SecLang rule set.
  std::shared_ptr<Antlr4::Parser> parser_;

  // Default actions defined in the SecDefaultAction directive
  // The default action is executed in the same phase as the rule that is matched and before
  // evaluating the rule's actions.
  std::array<const Rule*, PHASE_TOTAL> default_actions_{nullptr};

  // Even though the parser parses the rule set and stores the parsed rules, the parser  is not used
  // to evaluate the rules. Because each phase has a separate rule set, for performance reasons, we
  // store the each phase's rule pointers in an array.
  std::array<std::vector<const Rule*>, PHASE_TOTAL> rules_;

  std::unordered_map<std::string, Marker&> markers_;
  mutable PersistentStorage::Storage storage_;
};
} // namespace Wge