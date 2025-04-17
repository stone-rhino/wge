#pragma once

#include <functional>
#include <memory>
#include <optional>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>

#include "common/evaluate_result.h"
#include "common/ragel/multi_part.h"
#include "common/ragel/query_param.h"
#include "common/ragel/xml.h"
#include "common/variant.h"
#include "config.h"
#include "http_extractor.h"
#include "variable/full_name.h"

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
  // The connection info
  // At the ProcessConnection method, we store the downstream ip, downstream port, upstream ip, and
  // upstream port.
  struct ConnectionInfo {
    std::string_view downstream_ip_;
    short downstream_port_;
    std::string_view upstream_ip_;
    short upstream_port_;
  };

  // The request line info
  // At the ProcessUri method, we will parse the request line and store the method, path, query,
  // protocol, and version.
  struct RequestLineInfo {
    std::string method_;
    std::string_view uri_raw_;
    std::string_view uri_;
    std::string_view relative_uri_;
    std::string_view query_;
    std::string protocol_;
    std::string version_;
    Common::Ragel::QueryParam query_params_;
  };

  struct ResponseLineInfo {
    std::string_view status_code_;
    std::string_view protocol_;
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
   * @param request_line the request line. include method, path, query, protocol, version.
   * E.g. GET / HTTP/1.1
   */
  void processUri(std::string_view request_line);

  /**
   * Process the uri info.
   * @param uri the uri. E.g. /hello/world
   * @param method the method. E.g. GET
   * @param version the version. E.g. 1.1
   */
  void processUri(std::string_view uri, std::string_view method, std::string_view version);

  /**
   * Process the request headers.
   * @param request_header_find the header find function.
   * @param request_header_traversal the header traversal function.
   * @param request_header_count the count of the headers.
   * @param log_callback the log callback. if the rule is matched, the log_callback will be called.
   * @return true if the request is safe, false otherwise that means need to deny the request.
   */
  bool processRequestHeaders(HeaderFind request_header_find,
                             HeaderTraversal request_header_traversal, size_t request_header_count,
                             std::function<void(const Rule&)> log_callback);

  /**
   * Process the request body.
   * @param body_extractor the request body extractor.
   * @param log_callback the log callback. if the rule is matched, the log_callback will be called.
   * @return true if the request is safe, false otherwise that means need to deny the request.
   */
  bool processRequestBody(BodyExtractor body_extractor,
                          std::function<void(const Rule&)> log_callback);

  /**
   * Process the response headers.
   * @param status_code the status code of the response. E.g. 200
   * @param protocol the protocol of the response. E.g. HTTP/1.1
   * @param response_header_find the header find function.
   * @param response_header_traversal the header traversal function.
   * @param response_header_count the count of the headers.
   * @param log_callback the log callback. if the rule is matched, the log_callback will be called.
   * @return true if the request is safe, false otherwise that means need to deny the request.
   */
  bool processResponseHeaders(std::string_view status_code, std::string_view protocol,
                              HeaderFind response_header_find,
                              HeaderTraversal response_header_traversal,
                              size_t response_header_count,
                              std::function<void(const Rule&)> log_callback);

  /**
   * Process the response body.
   * @param body_extractor the response body extractor.
   * @param log_callback the log callback. if the rule is matched, the log_callback will be called.
   * @return true if the request is safe, false otherwise that means need to deny the request.
   */
  bool processResponseBody(BodyExtractor body_extractor,
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
   * Get the variables that the value is not empty in the transient transaction collection.
   * @return the variables. the first element is the name of the variable, and the second element is
   * the value of the variable.
   */
  std::vector<std::pair<std::string_view, Common::Variant*>> getVariables();

  /**
   * Get the count of the variables, which the value is not empty in the transient transaction
   * collection.
   * @return the count of the variables.
   */
  int getVariablesCount() const;

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
   * add a string that is captured by the operator.
   * @param value the matched value
   * @note the maximum number of matched strings is 100. if greater than 100, the value will be
   * ignored.
   */
  void addCapture(Common::EvaluateResults::Element&& value);

  /**
   * Get the captured string that is captured by the operator.
   * @param index the index of the matched string.the range is [0, 99].
   * @return the matched string.
   */
  const Common::Variant& getCapture(size_t index) const;

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
  const std::string_view getUniqueId();

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

  /**
   * Get the message macro expanded of current matched rule.
   * @return the message macro expanded of current matched rule.
   * @note We must copy the result to  another buffer if we want to store the result and use it
   * later. Because the result is a shared buffer that will be updated by the next matched rule.
   */
  const std::string& getMsgMacroExpanded() const { return msg_macro_expanded_.string_buffer_; }

  /**
   * Get the log data macro expanded of current matched rule.
   * @return the log data macro expanded of current matched rule.
   * @note We must copy the result to  another buffer if we want to store the result and use it
   * later. Because the result is a shared buffer that will be updated by the next matched rule.
   */
  const std::string& getLogDataMacroExpanded() const {
    return log_data_macro_expanded_.string_buffer_;
  }

  /**
   * Set the message macro expanded of current matched rule.
   * @param msg_macro_expanded the message macro expanded of current matched rule.
   */
  void setMsgMacroExpanded(Common::EvaluateResults::Element&& msg_macro_expanded) {
    msg_macro_expanded_ = std::move(msg_macro_expanded);
  }

  /**
   * Set the log data macro expanded of current matched rule.
   * @param log_data_macro_expanded the log data macro expanded of current matched rule.
   */
  void setLogDataMacroExpanded(Common::EvaluateResults::Element&& log_data_macro_expanded) {
    log_data_macro_expanded_ = std::move(log_data_macro_expanded);
  }

  /**
   * Add a matched variable.
   * Use for MATCHED_VAR, MATCHED_VARS, MATCHED_VAR_NAME, MATCHED_VARS_NAMES.
   * @param variable the matched variable.
   * @param result the result of the matched variable.
   */
  void pushMatchedVariable(const Variable::VariableBase* variable,
                           Common::EvaluateResults::Element&& result) {
    matched_variables_.emplace_back(variable, std::move(result));
  }

  /**
   * Get the matched variables(MATCHED_VAR, MATCHED_VARS, MATCHED_VAR_NAME, MATCHED_VARS_NAMES).
   * @return the matched variables.
   */
  const std::vector<std::pair<const Variable::VariableBase*, Common::EvaluateResults::Element>>&
  getMatchedVariables() const {
    return matched_variables_;
  }

  /**
   * Get the transformation cache.
   * @return the transformation cache.
   */
  std::unordered_map<
      Variable::FullName,
      std::unordered_map<const char*, std::optional<Common::EvaluateResults::Element>>>&
  getTransformCache() {
    return transform_cache_;
  }

  std::unordered_map<std::string_view, std::string_view>& getCookies() {
    initCookies();
    return cookies_;
  }

  /**
   * Get the connection info.
   * @return the connection info.
   */
  const ConnectionInfo& getConnectionInfo() const { return connection_info_; }

  /**
   * Get the raw request line.
   * @return the string view of the raw request line.
   */
  std::string_view getRequestLine() const { return request_line_; }

  /**
   * Get the request line info.
   * @return the request line info that parsed from the raw request line.
   */
  const RequestLineInfo& getRequestLineInfo() const { return requset_line_info_; }

  /**
   * Get the response line info.
   * @return the response line info
   */
  const ResponseLineInfo& getResponseLineInfo() const { return response_line_info_; }

  const Common::Ragel::QueryParam& getBodyQueryParam() const { return body_query_param_; }

  const Common::Ragel::MultiPart& getBodyMultiPart() const { return body_multi_part_; }

  const Common::Ragel::Xml& getBodyXml() const { return body_xml_; }

  const std::string& getReqBodyErrorMsg() const { return req_body_error_msg_; }

private:
  class RandomInitHelper {
  public:
    RandomInitHelper() { ::srand(::time(nullptr)); }
  };

  void initUniqueId();

  inline bool process(int phase);

  inline std::optional<size_t> getLocalVariableIndex(const std::string& key, bool force_create);

  void initCookies();

  inline std::optional<bool> doDisruptive(const Rule& rule, const Rule* default_action) const;

private:
  std::string unique_id_;
  HttpExtractor extractor_;
  const Engine& engine_;
  std::vector<Common::EvaluateResults::Element> tx_variables_;
  std::unordered_map<std::string, size_t> local_tx_variable_index_;
  std::unordered_map<size_t, std::string> local_tx_variable_index_reverse_;
  const size_t literal_key_size_;
  static constexpr int max_capture_size_{100};
  std::vector<Common::EvaluateResults::Element> captured_;
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
  using MatchedVariable =
      std::pair<const Variable::VariableBase*, Common::EvaluateResults::Element>;
  std::vector<MatchedVariable> matched_variables_;
  Common::EvaluateResults::Element msg_macro_expanded_;
  Common::EvaluateResults::Element log_data_macro_expanded_;
  std::unordered_map<
      Variable::FullName,
      std::unordered_map<const char*, std::optional<Common::EvaluateResults::Element>>>
      transform_cache_;
  bool init_cookies_{false};
  std::unordered_map<std::string_view, std::string_view> cookies_;

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
  std::string_view request_line_;
  std::string request_line_buffer_;
  RequestLineInfo requset_line_info_;
  ResponseLineInfo response_line_info_;
  Common::Ragel::QueryParam body_query_param_;
  Common::Ragel::MultiPart body_multi_part_;
  Common::Ragel::Xml body_xml_;
  std::string req_body_error_msg_;
};

using TransactionPtr = std::unique_ptr<Transaction>;
} // namespace SrSecurity