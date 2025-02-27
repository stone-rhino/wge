#pragma once

#include <functional>
#include <memory>
#include <optional>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>

#include "common/variant.h"
#include "config.h"
#include "http_extractor.h"

namespace SrSecurity {
class Engine;

class Rule;

namespace Variable {
class VariableBase;
} // namespace Variable

class Transaction final {
  friend class Engine;

protected:
  Transaction(const Engine& engin);

public:
  /**
   * Process the connection info.
   * @param conn_extractor the connection info extractor.
   */
  void processConnection(ConnectionExtractor conn_extractor);

  /**
   * Process the uri info.
   * @param uri_extractor the uri info extractor.
   */
  void processUri(UriExtractor uri_extractor);

  /**
   * Process the request headers.
   * @param header_extractor the request headers extractor.
   * @param log_callback the log callback. if the rule is matched, the log_callback will be called.
   */
  void processRequestHeaders(HeaderExtractor header_extractor,
                             std::function<void(const Rule&)> log_callback);

  /**
   * Process the request body.
   * @param body_extractor the request body extractor.
   * @param log_callback the log callback. if the rule is matched, the log_callback will be called.
   */
  void processRequestBody(BodyExtractor body_extractor,
                          std::function<void(const Rule&)> log_callback);

  /**
   * Process the response headers.
   * @param header_extractor the response headers extractor.
   * @param log_callback the log callback. if the rule is matched, the log_callback will be called.
   */
  void processResponseHeaders(HeaderExtractor header_extractor,
                              std::function<void(const Rule&)> log_callback);

  /**
   * Process the response body.
   * @param body_extractor the response body extractor.
   * @param log_callback the log callback. if the rule is matched, the log_callback will be called.
   */
  void processResponseBody(BodyExtractor body_extractor,
                           std::function<void(const Rule&)> log_callback);

public:
  /**
   * Create a variable in the transient transaction collection
   * @param name the name of the variable.
   * @param value the value of the variable.
   */
  void createVariable(std::string&& name, Common::Variant&& value);

  /**
   * Remove a variable from the transient transaction collection
   * @param name the name of the variable.
   */
  void removeVariable(const std::string& name);

  /**
   * Increase the value of a variable in the transient transaction collection
   * @param name the name of the variable.
   * @param value the int value to increase.
   */
  void increaseVariable(const std::string& name, int value = 1);

  /**
   * Get the value of a variable in the transient transaction collection
   * @param name the name of the variable.
   * @return the value of the variable. if the variable does not exist, return an empty variant.
   */
  const Common::Variant& getVariable(const std::string& name) const;

  /**
   * Set the string value of a variable in the transient transaction collection
   * @param name the name of the variable.
   * @param value the string value of the variable.
   */
  void setVariable(const std::string& name, std::string&& value);

  /**
   * Set the int value of a variable in the transient transaction collection
   * @param name the name of the variable.
   * @param value the int value of the variable.
   */
  void setVariable(const std::string& name, int value);

  /**
   * Check if the variable exists in the transient transaction collection
   * @param name the name of the variable.
   * @return true if the variable exists, false otherwise.
   */
  bool hasVariable(const std::string& name) const;

  /**
   * Set the matched string that is captured by the operator.
   * @param index the index of the matched string.the range is [0, 99].
   * @param value the reference of the matched string.
   */
  void setMatched(size_t index, const std::string_view& value);

  /**
   * Get the matched string that is captured by the operator.
   * @param index the index of the matched string.the range is [0, 99].
   * @return the matched string.if the matched string does not exist, return nullptr.
   */
  const std::string_view* getMatched(size_t index) const;

  /**
   * Get the HTTP extractor.
   * @return the HTTP extractor.
   */
  const HttpExtractor& httpExtractor() const { return extractor_; }

  /**
   * Set the request body processor.
   * @param type the request body processor.
   */
  void setRequestBodyProcessor(BodyProcessorType type) { request_body_processor_ = type; }

  /**
   * Get the request body processor.
   * @return the request body processor.
   */
  BodyProcessorType getRequestBodyProcessor() const { return *request_body_processor_; }

  /**
   * Get the Unique ID of the transaction.
   * @return the Unique ID of the transaction.
   */
  const std::string& getUniqueId() const { return unique_id_; }

  /**
   * Get the engine.
   * @return the engine.
   */
  const Engine& getEngine() const { return engin_; }

  /**
   * Remove the rule.
   * The rule will be removed from the transaction instance, and the rule will not be evaluated. The
   * other transaction instances running in parallel will be unaffected.
   * @param rules the rules that will be removed.
   */
  void removeRule(const std::array<std::unordered_set<const Rule*>, PHASE_TOTAL>& rules);

  /**
   * Remove the rule's target.
   * The rule's target will be removed from the transaction instance, and the rule will not be
   * evaluated. The other transaction instances running in parallel will be unaffected.
   * @param rule the rule that will be remove target.
   * @param variables the variables that will be removed.
   */
  void removeRuleTarget(const std::array<std::unordered_set<const Rule*>, PHASE_TOTAL>& rules,
                        const std::vector<std::shared_ptr<Variable::VariableBase>>& variables);

private:
  class RandomInitHelper {
  public:
    RandomInitHelper() { ::srand(::time(nullptr)); }
  };

  void initUniqueId();

  inline void process(int phase);

  /**
   * Evaluate the rules.
   * @param rules the rules that will be evaluated.
   * @return true if the all of the rules evaluated over, false otherwise that means the rules have
   * been reordered, we need call this method again.
   */
  inline bool evaluateRules(const std::vector<const Rule*>& rules);

private:
  std::string unique_id_;
  HttpExtractor extractor_;
  const Engine& engin_;
  std::unordered_map<std::string, Common::Variant> tx_;
  std::array<std::string_view, 100> matched_;
  static const RandomInitHelper random_init_helper_;
  std::function<void(const Rule&)> log_callback_;

  // All of the transaction instances share the same rule instances, and each transaction instance
  // may be removed or updated some different rules by the ctl action. So, we need to mark the rules
  // that need to be removed or updated in local.
  // The allocation memory behavior is lazy, and only the rules that need to be removed or updated
  // will be allocated memory that is same as the engin.rules() size.
  std::array<std::vector<bool>, PHASE_TOTAL> rule_remove_flags_;
  std::array<const std::vector<std::shared_ptr<Variable::VariableBase>>, PHASE_TOTAL>
      rule_remove_targets_;

  // Current evaluation state
  int current_phase_{1};
  const std::vector<const Rule*>* current_rules_{nullptr};
  size_t current_rule_index_{0};

  // ctl
private:
  std::optional<AuditLogConfig::AuditEngine> audit_engine_;
  std::optional<AuditLogConfig::AuditLogPart> audit_log_part_;
  std::optional<EngineConfig::Option> request_body_access_;
  std::optional<BodyProcessorType> request_body_processor_;
  std::optional<EngineConfig::Option> rule_engine_;
};

using TransactionPtr = std::unique_ptr<Transaction>;
} // namespace SrSecurity