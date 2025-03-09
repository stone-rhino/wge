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
  Transaction(const Engine& engin, size_t literal_key_size);

public:
  // The evaluated buffer
  // Each variable, macro, and action will be evaluated in the transaction. The result of the
  // evaluation will be stored in the evaluated buffer.
  class EvaluatedBuffer {
  public:
    EvaluatedBuffer() = default;
    EvaluatedBuffer(const EvaluatedBuffer&) = delete;

  public:
    const Common::Variant& set() {
      variant_ = EMPTY_VARIANT;
      return variant_;
    }

    const Common::Variant& set(int value) {
      variant_ = value;
      return variant_;
    }

    const Common::Variant& set(std::string_view value) {
      string_buffer_ = value;
      variant_ = string_buffer_;
      return variant_;
    }

    const Common::Variant& set(std::string&& value) {
      string_buffer_ = std::move(value);
      variant_ = string_buffer_;
      return variant_;
    }

  private:
    Common::Variant variant_;
    std::string string_buffer_;
  };

  enum class EvaluatedBufferType { Variable = 0, Macro, Msg, LogData, EvaluatedBufferTypeTotal };

  // The connection info
  // At the ProcessConnection method, we store the downstream ip, downstream port, upstream ip, and
  // upstream port.
  struct ConnectionInfo {
    std::string_view downstream_ip_;
    short downstream_port_;
    std::string_view upstream_ip_;
    short upstream_port_;
  };

  // The URI info
  // At the ProcessUri method, we will parse the uri and store the method, path, query, protocol,
  // and version.
  struct UriInfo {
    std::string method_;
    std::string_view path_;
    std::string_view query_;
    std::string protocol_;
    std::string version_;
  };

public:
  /**
   * Process the connection info.
   * @param downstream_ip the downstream ip.
   * @param downstream_port the downstream port.
   * @param upstream_ip the upstream ip.
   * @param upstream_port the upstream port.
   */
  void processConnection(std::string_view downstream_ip, short downstream_port,
                         std::string_view upstream_ip, short upstream_port);

  /**
   * Process the uri info.
   * @param uri the uri info. include method, path, query, protocol, version. E.g. GET / HTTP/1.1
   */
  void processUri(std::string_view uri);

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
   * Create or update a variable in the transient transaction collection.
   *
   * Used for create a variable that the key of the variable can be evaluated at parse time. E.g.
   * tx.foo=1. In the Example, the key is literal string "foo", and we can calculate the index of
   * the variable by the order of the key in the collection. This solution is more efficient than
   * the other solution that the key is a macro that only can be evaluated at runtime. Because we
   * must calculate the hash value of the key every time when we want to get the variable.
   * @param index the index of the variable.
   * @param value the value of the variable.
   */
  void setVariable(size_t index, const Common::Variant& value);

  /**
   * Create or update a variable in the transient transaction collection.
   *
   * Used for create a variable that the key of the variable can't be evaluated at parse time.
   * Such as tx.%{tx.foo}=1. In the Example, the key is a macro that only can be evaluated at
   * runtime. Because we must calculate the hash value of the key every time when we want to get the
   * variable, this solution is less efficient than the other solution that the key is a literal
   * string.
   * @param name the name of the variable.
   * @param value the value of the variable.
   */
  void setVariable(std::string&& name, const Common::Variant& value);

  /**
   * Remove a variable from the transient transaction collection
   *
   * Used for remove a variable that the key of the variable can be evaluated at parse time.
   * Please refer to the createVariable method for more details.
   * @param index the index of the variable.
   */
  void removeVariable(size_t index);

  /**
   * Remove a variable from the transient transaction collection
   *
   * Used for remove a variable that the key of the variable can't be evaluated at parse time.
   * Please refer to the createVariable method for more details.
   * @param name the name of the variable.
   * @note This method only used in the test. An efficient and rational design should not call this
   * method in the worker thread.
   */
  void removeVariable(const std::string& name);

  /**
   * Increase the value of a variable in the transient transaction collection
   *
   * Used for increase the value of a variable that the key of the variable can be evaluated at
   * parse time. Please refer to the createVariable method for more details.
   * @param index the index of the variable.
   * @param value the int value to increase.
   */
  void increaseVariable(size_t index, int value = 1);

  /**
   * Increase the value of a variable in the transient transaction collection
   *
   * Used for increase the value of a variable that the key of the variable can't be evaluated at
   * parse time. Please refer to the createVariable method for more details.
   * @param name the name of the variable.
   * @param value the int value to increase.
   */
  void increaseVariable(const std::string& name, int value = 1);

  /**
   * Get the value of a variable in the transient transaction collection
   *
   * Used for get the value of a variable that the key of the variable can be evaluated at parse
   * time. Please refer to the createVariable method for more details.
   * @param index the index of the variable.
   * @return the value of the variable. if the variable does not exist, return an empty variant.
   */
  const Common::Variant& getVariable(size_t index) const;

  /**
   * Get the value of a variable in the transient transaction collection
   *
   * Used for get the value of a variable that the key of the variable can't be evaluated at parse
   * time. Please refer to the createVariable method for more details.
   * @param name the name of the variable.
   * @return the value of the variable. if the variable does not exist, return an empty variant.
   */
  const Common::Variant& getVariable(const std::string& name);

  /**
   * Check if the variable exists in the transient transaction collection
   *
   * Used for check if the variable exists that the key of the variable can be evaluated at parse
   * time. Please refer to the createVariable method for more details.
   * @param index the index of the variable.
   * @return true if the variable exists, false otherwise.
   */
  bool hasVariable(size_t index) const;

  /**
   * Check if the variable exists in the transient transaction collection
   *
   * Used for check if the variable exists that the key of the variable can't be evaluated at
   * parse time. Please refer to the createVariable method for more details.
   * @param name the name of the variable.
   * @return true if the variable exists, false otherwise.
   */
  bool hasVariable(const std::string& name) const;

  /**
   * Set the matched string that is captured by the operator.
   * @param index the index of the matched string.the range is [0, 99].
   * @param value the reference of the matched string.
   */
  void setMatched(size_t index, std::string_view value);

  /**
   * Get the matched string that is captured by the operator.
   * @param index the index of the matched string.the range is [0, 99].
   * @return the matched value. if the matched string does not exist, return empty variant.
   */
  const Common::Variant& getMatched(size_t index) const;

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
  const std::string_view getUniqueId() const { return unique_id_; }

  /**
   * Get the engine.
   * @return the engine.
   */
  const Engine& getEngine() const { return engine_; }

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

  EvaluatedBuffer& getEvaluatedBuffer(EvaluatedBufferType type) {
    return evaluated_buffers_[static_cast<size_t>(type)];
  }

  const ConnectionInfo& getConnectionInfo() const { return connection_info_; }

  std::string_view getUri() const { return uri_; }

  const UriInfo& getUriInfo() const { return uri_info_; }

private:
  class RandomInitHelper {
  public:
    RandomInitHelper() { ::srand(::time(nullptr)); }
  };

  void initUniqueId();

  inline void process(int phase);

  inline std::optional<size_t> getLocalVariableIndex(const std::string& key, bool force_create);

private:
  std::string unique_id_;
  HttpExtractor extractor_;
  const Engine& engine_;
  std::vector<Common::Variant> tx_variables_;
  std::vector<std::string> tx_variables_buffer_;
  std::unordered_map<std::string, size_t> local_tx_variable_index_;
  const size_t literal_key_size_;
  std::array<Common::Variant, 100> matched_;
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
  // The evaluated buffer is shared by variables,macros, and actions. So, we need to copy the value
  // to a local variable if we want to use it
  std::array<EvaluatedBuffer, static_cast<size_t>(EvaluatedBufferType::EvaluatedBufferTypeTotal)>
      evaluated_buffers_;

  // ctl
private:
  std::optional<AuditLogConfig::AuditEngine> audit_engine_;
  std::optional<AuditLogConfig::AuditLogPart> audit_log_part_;
  std::optional<EngineConfig::Option> request_body_access_;
  std::optional<BodyProcessorType> request_body_processor_;
  std::optional<EngineConfig::Option> rule_engine_;

  // The http info
private:
  ConnectionInfo connection_info_;
  std::string_view uri_;
  UriInfo uri_info_;
};

using TransactionPtr = std::unique_ptr<Transaction>;
} // namespace SrSecurity