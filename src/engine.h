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

namespace SrSecurity::Antlr4 {
class Parser;
}

namespace SrSecurity {
class Engine {
public:
  Engine();

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
   * @param level the log level
   * @param log_file the log file path. If it is empty, the log will be output to the console
   * @note must call once before call makeTransaction, and only once in the life of the engine
   * instance.
   */
  void init(spdlog::level::level_enum level = spdlog::level::info,
            const std::string& log_file = "");

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
   * Get the parser
   * @return reference of parser
   */
  const Antlr4::Parser& parser() const { return *parser_; }

  std::optional<const std::vector<const Rule*>::iterator> marker(const std::string& name,
                                                                 int phase) const;

  /**
   * Get persistent storage
   * @return reference of persistent storage
   */
  PersistentStorage::Storage& storage() { return storage_; }

private:
  void initDefaultActions();
  void initRules();
  void initMakers();

private:
  constexpr static size_t phase_total_ = 5;
  std::shared_ptr<Antlr4::Parser> parser_;
  std::array<const Rule*, phase_total_> default_actions_{nullptr};
  std::array<std::vector<const Rule*>, phase_total_> rules_;
  std::unordered_map<std::string, Marker&> markers_;
  PersistentStorage::Storage storage_;
};
} // namespace SrSecurity