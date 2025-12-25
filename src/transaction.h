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

#include <forward_list>
#include <functional>
#include <memory>
#include <optional>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>

#include <boost/unordered/unordered_flat_map.hpp>
#include <boost/unordered/unordered_flat_set.hpp>

#include "common/evaluate_result.h"
#include "common/property_tree.h"
#include "common/ragel/json.h"
#include "common/ragel/multi_part.h"
#include "common/ragel/query_param.h"
#include "common/ragel/xml.h"
#include "common/variant.h"
#include "config.h"
#include "http_extractor.h"
#include "macro/macro_base.h"
#include "persistent_storage/storage.h"
#include "variable/full_name.h"

namespace Wge {
class Engine;

class Rule;

namespace Variable {
class VariableBase;
} // namespace Variable

namespace Transformation {
class TransformBase;
} // namespace Transformation

class Transaction final {
  friend class Engine;

protected:
  Transaction(const Engine& engin);

public:
  // The connection info
  // At the ProcessConnection method, we store the downstream ip, downstream port, upstream ip, and
  // upstream port.
  struct ConnectionInfo {
    std::string_view downstream_ip_;
    std::string_view upstream_ip_;
    short downstream_port_;
    short upstream_port_;
  };

  // The request line info
  // At the ProcessUri method, we will parse the request line and store the method, path, query,
  // protocol, and version.
  struct RequestLineInfo {
    std::string_view method_;
    std::string_view uri_raw_;
    std::string_view uri_;
    std::string_view relative_uri_;
    std::string_view query_;
    std::string_view protocol_;
    std::string_view version_;
    std::string_view base_name_;
    Common::Ragel::QueryParam query_params_;
  };

  struct ResponseLineInfo {
    std::string_view status_code_;
    std::string_view protocol_;
  };

  // For the MATCHED_VAR_NAME, MATCHED_VAR, MATCHED_VARS_NAMES, MATCHED_VARS
  struct MatchedVariable {
    // The matched variable
    const Variable::VariableBase* variable_;

    // The original value of the matched variable
    Common::EvaluateElement original_value_;

    // The transformed value of the matched variable
    Common::EvaluateElement transformed_value_;

    // The captured value of the matched variable
    std::string_view captured_value_;

    // The list of transformations that applied to the matched variable
    std::list<const Transformation::TransformBase*> transform_list_;

    MatchedVariable(const Variable::VariableBase* variable,
                    const Common::EvaluateElement& original_value,
                    const Common::EvaluateElement& transformed_value,
                    std::string_view captured_value,
                    std::list<const Transformation::TransformBase*>&& transform_list)
        : variable_(variable), original_value_(original_value),
          transformed_value_(transformed_value), captured_value_(captured_value),
          transform_list_(std::move(transform_list)) {}
  };

  struct TransformCacheKey {
    std::string_view input_data_view_;
    const char* transformation_name_;

    TransformCacheKey(std::string_view input_data_view, const char* transformation_name)
        : input_data_view_(input_data_view), transformation_name_(transformation_name) {}

    bool operator==(const TransformCacheKey& other) const {
      return input_data_view_.data() == other.input_data_view_.data() &&
             input_data_view_.size() == other.input_data_view_.size() &&
             transformation_name_ == other.transformation_name_;
    }

    friend size_t hash_value(const TransformCacheKey& key) {
      return std::hash<const void*>()(key.input_data_view_.data()) ^
             (std::hash<size_t>()(key.input_data_view_.size()) << 2) ^
             (std::hash<const char*>()(key.transformation_name_) << 4);
    }
  };

  using TransformCache =
      boost::unordered_flat_map<TransformCacheKey, std::optional<Common::EvaluateElement>>;

  /**
   * The log callback
   * @param rule the reference to the matched rule.
   * @param user_data the user data pointer.
   */
  using LogCallback = void (*)(const Rule& rule, void* user_data);

  /**
   * The additional condition callback
   * @param rule the reference to the matched rule.
   * @param variable the reference to the variable.
   * @param value the value of the variable.
   * @param user_data the user data pointer.
   * @return true if the additional condition is matched, false otherwise.
   */
  using AdditionalCondCallback = bool (*)(const Rule& rule, const Variable::VariableBase& variable,
                                          std::string_view value, void* user_data);
  // Process the transaction
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
   * @param log_user_data the user data pointer for the log callback.
   * @param additional_cond an "AND" logic based on the original logic of the rule, only if both
   * match successfully is the final result true.
   * @param additional_cond_user_data the user data pointer for the additional condition callback.
   * @return true if the request is safe, false otherwise that means need to deny the request.
   */
  bool processRequestHeaders(HeaderFind request_header_find,
                             HeaderTraversal request_header_traversal, size_t request_header_count,
                             LogCallback log_callback = nullptr, void* log_user_data = nullptr,
                             AdditionalCondCallback additional_cond = nullptr,
                             void* additional_cond_user_data = nullptr);

  /**
   * Process the request body.
   * @param body the request body.
   * @param log_callback the log callback. if the rule is matched, the log_callback will be called.
   * @param log_user_data the user data pointer for the log callback.
   * @param additional_cond an "AND" logic based on the original logic of the rule, only if both
   * match successfully is the final result true.
   * @param additional_cond_user_data the user data pointer for the additional condition callback.
   * @return true if the request is safe, false otherwise that means need to deny the request.
   */
  bool processRequestBody(std::string_view body, LogCallback log_callback = nullptr,
                          void* log_user_data = nullptr,
                          AdditionalCondCallback additional_cond = nullptr,
                          void* additional_cond_user_data = nullptr);

  /**
   * Process the response headers.
   * @param status_code the status code of the response. E.g. 200
   * @param protocol the protocol of the response. E.g. HTTP/1.1
   * @param response_header_find the header find function.
   * @param response_header_traversal the header traversal function.
   * @param response_header_count the count of the headers.
   * @param log_callback the log callback. if the rule is matched, the log_callback will be called.
   * @param log_user_data the user data pointer for the log callback.
   * @param additional_cond an "AND" logic based on the original logic of the rule, only if both
   * match successfully is the final result true.
   * @param additional_cond_user_data the user data pointer for the additional condition callback.
   * @return true if the request is safe, false otherwise that means need to deny the request.
   */
  bool processResponseHeaders(std::string_view status_code, std::string_view protocol,
                              HeaderFind response_header_find,
                              HeaderTraversal response_header_traversal,
                              size_t response_header_count, LogCallback log_callback = nullptr,
                              void* log_user_data = nullptr,
                              AdditionalCondCallback additional_cond = nullptr,
                              void* additional_cond_user_data = nullptr);

  /**
   * Process the response body.
   * @param body the response body.
   * @param log_callback the log callback. if the rule is matched, the log_callback will be called.
   * @param log_user_data the user data pointer for the log callback.
   * @param additional_cond an "AND" logic based on the original logic of the rule, only if both
   * match successfully is the final result true.
   * @param additional_cond_user_data the user data pointer for the additional condition callback.
   * @return true if the request is safe, false otherwise that means need to deny the request.
   */
  bool processResponseBody(std::string_view body, LogCallback log_callback = nullptr,
                           void* log_user_data = nullptr,
                           AdditionalCondCallback additional_cond = nullptr,
                           void* additional_cond_user_data = nullptr);

  // Http transaction data
public:
  const HttpExtractor& httpExtractor() const { return extractor_; }
  const ConnectionInfo& getConnectionInfo() const { return connection_info_; }
  std::string_view getRequestLine() const { return request_line_; }
  const RequestLineInfo& getRequestLineInfo() const { return request_line_info_; }
  std::string_view getRequestBody() const { return request_body_; }
  std::string_view getResponseBody() const { return response_body_; }
  const ResponseLineInfo& getResponseLineInfo() const { return response_line_info_; }
  const Common::Ragel::QueryParam& getBodyQueryParam() const { return body_query_param_; }
  const Common::Ragel::MultiPart& getBodyMultiPart() const { return body_multi_part_; }
  const Common::Ragel::Xml& getBodyXml() const { return body_xml_; }
  const Common::Ragel::Json& getBodyJson() const { return body_json_; }
  const std::string& getReqBodyErrorMsg() const { return req_body_error_msg_; }
  const std::unordered_multimap<std::string_view, std::string_view>& getCookies() const {
    initCookies();
    return *cookies_;
  }

  // Current evaluation state
public:
  const Engine& getEngine() const { return engine_; }
  const Rule* getCurrentEvaluateRule() const { return current_rule_; }
  void setCurrentEvaluateRule(const Rule* rule) { current_rule_ = rule; }

  /**
   * Create or update a variable in the transient transaction collection.
   *
   * Used for create a variable that the key of the variable can be evaluated at parse time. E.g.
   * tx.foo=1. In the Example, the key is literal string "foo", and we can calculate the index of
   * the variable by the order of the key in the collection. This solution is more efficient than
   * the other solution that the key is a macro that only can be evaluated at runtime. Because we
   * must calculate the hash value of the key every time when we want to get the variable.
   * @param ns the variable namespace.
   * @param index the index of the variable.
   * @param value the value of the variable.
   */
  void setVariable(const std::string& ns, size_t index, const Common::Variant& value);

  /**
   * Create or update a variable in the transient transaction collection.
   *
   * Used for create a variable that the key of the variable can't be evaluated at parse time.
   * Such as tx.%{tx.foo}=1. In the Example, the key is a macro that only can be evaluated at
   * runtime. Because we must calculate the hash value of the key every time when we want to get the
   * variable, this solution is less efficient than the other solution that the key is a literal
   * string.
   * @param ns the variable namespace.
   * @param name the name of the variable.
   * @param value the value of the variable.
   */
  void setVariable(const std::string& ns, std::string&& name, const Common::Variant& value);

  /**
   * Remove a variable from the transient transaction collection
   *
   * Used for remove a variable that the key of the variable can be evaluated at parse time.
   * Please refer to the createVariable method for more details.
   * @param ns the variable namespace.
   * @param index the index of the variable.
   */
  void removeVariable(const std::string& ns, size_t index);

  /**
   * Remove a variable from the transient transaction collection
   *
   * Used for remove a variable that the key of the variable can't be evaluated at parse time.
   * Please refer to the createVariable method for more details.
   * @param ns the variable namespace.
   * @param name the name of the variable.
   * @note This method only used in the test. An efficient and rational design should not call this
   * method in the worker thread.
   */
  void removeVariable(const std::string& ns, const std::string& name);

  /**
   * Increase the value of a variable in the transient transaction collection
   *
   * Used for increase the value of a variable that the key of the variable can be evaluated at
   * parse time. Please refer to the createVariable method for more details.
   * @param ns the variable namespace.
   * @param index the index of the variable.
   * @param value the int64_t value to increase.
   */
  void increaseVariable(const std::string& ns, size_t index, int64_t value = 1);

  /**
   * Increase the value of a variable in the transient transaction collection
   *
   * Used for increase the value of a variable that the key of the variable can't be evaluated at
   * parse time. Please refer to the createVariable method for more details.
   * @param ns the variable namespace.
   * @param name the name of the variable.
   * @param value the int64_t value to increase.
   */
  void increaseVariable(const std::string& ns, const std::string& name, int64_t value = 1);

  /**
   * Get the value of a variable in the transient transaction collection
   *
   * Used for get the value of a variable that the key of the variable can be evaluated at parse
   * time. Please refer to the createVariable method for more details.
   * @param ns the variable namespace.
   * @param index the index of the variable.
   * @return the value of the variable. if the variable does not exist, return an empty variant.
   */
  const Common::Variant& getVariable(const std::string& ns, size_t index) const;

  /**
   * Get the value of a variable in the transient transaction collection
   *
   * Used for get the value of a variable that the key of the variable can't be evaluated at parse
   * time. Please refer to the createVariable method for more details.
   * @param ns the variable namespace.
   * @param name the name of the variable.
   * @return the value of the variable. if the variable does not exist, return an empty variant.
   */
  const Common::Variant& getVariable(const std::string& ns, const std::string& name) const;

  /**
   * Get the variables that the value is not empty in the transient transaction collection.
   * @param ns the variable namespace.
   * @return the variables. the first element is the name of the variable, and the second element is
   * the value of the variable.
   */
  std::vector<std::pair<std::string_view, const Common::Variant*>>
  getVariables(const std::string& ns) const;

  /**
   * Get the count of the variables, which the value is not empty in the transient transaction
   * collection.
   * @param ns the variable namespace.
   * @return the count of the variables.
   */
  int64_t getVariablesCount(const std::string& ns) const;

  /**
   * Check if the variable exists in the transient transaction collection
   *
   * Used for check if the variable exists that the key of the variable can be evaluated at parse
   * time. Please refer to the createVariable method for more details.
   * @param ns the variable namespace.
   * @param index the index of the variable.
   * @return true if the variable exists, false otherwise.
   */
  bool hasVariable(const std::string& ns, size_t index) const;

  /**
   * Check if the variable exists in the transient transaction collection
   *
   * Used for check if the variable exists that the key of the variable can't be evaluated at
   * parse time. Please refer to the createVariable method for more details.
   * @param ns the variable namespace.
   * @param name the name of the variable.
   * @return true if the variable exists, false otherwise.
   */
  bool hasVariable(const std::string& ns, const std::string& name) const;

  /**
   * Set the captured string that is captured by the operator.
   * @param index the index of the matched string. The range is [0, 99].
   * @param value the matched value
   * @note the maximum number of matched strings is 100. if greater than 100, the value will be
   * ignored.
   */
  void setCapture(size_t index, std::string_view value);

  /**
   * Get the captured string that is captured by the operator.
   * @param index the index of the matched string.the range is [0, 99].
   * @return the matched string.
   */
  std::string_view getCapture(size_t index) const;

  /**
   * Add a matched variable.
   * Use for MATCHED_VAR, MATCHED_VARS, MATCHED_VAR_NAME, MATCHED_VARS_NAMES.
   * @param variable the matched variable.
   * @param rule_chain_index the chain index of the rule that matched this variable.
   * @param original_value the original value of the matched variable.
   * @param transformed_value the transformed value of the matched variable.
   * @param captured_value the captured value of the matched variable.
   * @param transform_list the list of transformations that applied to the matched variable.
   * @param result the result of the matched variable.
   */
  void pushMatchedVariable(const Variable::VariableBase* variable,
                           RuleChainIndexType rule_chain_index,
                           const Common::EvaluateElement& original_value,
                           const Common::EvaluateElement& transformed_value,
                           std::string_view captured_value,
                           std::list<const Transformation::TransformBase*>&& transform_list);

  /**
   * Get the matched variables(MATCHED_VAR, MATCHED_VARS, MATCHED_VAR_NAME, MATCHED_VARS_NAMES).
   * @return the matched variables.
   */
  const std::vector<MatchedVariable>& getMatchedVariables(int rule_chain_index) const {
    auto iter = matched_variables_.find(rule_chain_index);
    if (iter != matched_variables_.end()) {
      return iter->second;
    }

    static const std::vector<MatchedVariable> empty_vector;
    return empty_vector;
  }

  /**
   * Stage a matched operator property tree node. After staging, we should call
   * commitMatchedOPTree() to commit the staged matched operator property tree nodes into the main
   * matched operator, then we can use getMatchedOPTrees() to get the matched operator property tree
   * nodes.
   * @param optree the matched operator property tree node.
   */
  void stageMatchedOPTree(const Common::PropertyTree* optree) {
    temp_matched_optrees_.emplace_back(optree);
  }

  /**
   * Rollback the staged matched operator property tree nodes.
   */
  void rollbackMatchedOPTree() { temp_matched_optrees_.clear(); }

  /**
   * Commit the staged matched operator property tree nodes into the main matched operator property
   * tree nodes.
   * After calling this method, we can use getMatchedOPTrees() to get the matched operator property
   * tree nodes.
   * @param rule_chain_index the chain index of the rule that matched these nodes
   * @return the number of matched operator property tree nodes that are committed.
   */
  size_t commitMatchedOPTree(RuleChainIndexType rule_chain_index) {
    auto& optrees =
        matched_optrees_.try_emplace(rule_chain_index, std::vector<const Common::PropertyTree*>())
            .first->second;
    optrees.insert(optrees.end(), temp_matched_optrees_.begin(), temp_matched_optrees_.end());
    size_t count = temp_matched_optrees_.size();
    temp_matched_optrees_.clear();
    return count;
  }

  /**
   * Get the matched operator property tree nodes(MATCHED_OPTREE).
   * @return the matched operator property tree nodes.
   */
  const std::vector<const Common::PropertyTree*>& getMatchedOPTrees(int rule_chain_index) const {
    auto iter = matched_optrees_.find(rule_chain_index);
    if (iter != matched_optrees_.end()) {
      return iter->second;
    }

    static const std::vector<const Common::PropertyTree*> empty_vector;
    return empty_vector;
  }

  /**
   * Add a matched variable property tree node.
   * Use for MATCHED_VPTREE.
   * @param rule_chain_index the chain index of the rule that matched this node
   * @param vptree the matched variable property tree node.
   */
  void pushMatchedVPTree(RuleChainIndexType rule_chain_index, const Common::PropertyTree* vptree) {
    auto& vptrees =
        matched_vptrees_.try_emplace(rule_chain_index, std::vector<const Common::PropertyTree*>())
            .first->second;
    vptrees.emplace_back(vptree);
  }

  /**
   * Get the matched variable property tree nodes(MATCHED_VPTREE).
   * @return the matched variable property tree nodes.
   */
  const std::vector<const Common::PropertyTree*>& getMatchedVPTrees(int rule_chain_index) const {
    auto iter = matched_vptrees_.find(rule_chain_index);
    if (iter != matched_vptrees_.end()) {
      return iter->second;
    }

    static const std::vector<const Common::PropertyTree*> empty_vector;
    return empty_vector;
  }

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
   * Check if the rule's target is removed.
   * @param rule the rule that will be checked.
   * @param full_name the variable that specify by full name will be checked.
   * @return true if the rule's target is removed, false otherwise.
   */
  bool isRuleTargetRemoved(const Rule* rule, Variable::FullName full_name) const;

  /**
   * Get the message macro expanded of current matched rule.
   * @return the message macro expanded of current matched rule.
   * @note We must copy the result to  another buffer if we want to store the result and use it
   * later. Because the result is a shared buffer that will be updated by the next matched rule.
   */
  std::string_view getMsgMacroExpanded();

  /**
   * Get the log data macro expanded of current matched rule.
   * @return the log data macro expanded of current matched rule.
   * @note We must copy the result to  another buffer if we want to store the result and use it
   * later. Because the result is a shared buffer that will be updated by the next matched rule.
   */
  std::string_view getLogDataMacroExpanded();

  TransformCache& getTransformCache() { return transform_cache_; }

  std::string_view getPersistentStorageKey(PersistentStorage::Storage::Type type) const;

  void setPersistentStorageKey(PersistentStorage::Storage::Type type, const std::string& key) {
    persistent_storage_keys_[static_cast<size_t>(type)] = key;
  }

  void setPersistentStorageKey(PersistentStorage::Storage::Type type, const Macro::MacroBase* key) {
    persistent_storage_keys_[static_cast<size_t>(type)] = key;
  }

  // Configuration options by ctl action
public:
  void setRequestBodyProcessor(BodyProcessorType type) { request_body_processor_ = type; }
  BodyProcessorType getRequestBodyProcessor() const { return *request_body_processor_; }
  void setParseXmlIntoArgs(ParseXmlIntoArgsOption option) { parse_xml_into_args_ = option; }
  ParseXmlIntoArgsOption getParseXmlIntoArgs() const;

public:
  std::string_view getUniqueId() const;
  AdditionalCondCallback getAdditionalCond() const { return additional_cond_; }
  void* getAdditionalCondUserdata() const { return additional_cond_user_data_; }

  /**
   * Intern a string into the string pool.
   * @param str the string to be interned.
   * @return the string view of the interned string.
   */
  std::string_view internString(std::string&& str) {
    string_pool_.emplace_front(std::move(str));
    return string_pool_.front();
  }

private:
  void initUniqueId() const;
  inline bool process(RulePhaseType phase);
  inline std::optional<size_t> getLocalVariableIndex(const std::string& ns,
                                                     const std::string& key) const;
  inline size_t getOrCreateLocalVariableIndex(const std::string& ns, const std::string& key);
  void initCookies() const;
  inline std::optional<bool> doDisruptive(const Rule& rule, const Rule* default_action);

  // Http transaction data
private:
  HttpExtractor extractor_;
  ConnectionInfo connection_info_;
  std::string_view request_line_;
  RequestLineInfo request_line_info_;
  ResponseLineInfo response_line_info_;
  std::string_view request_body_;
  std::string_view response_body_;
  Common::Ragel::QueryParam body_query_param_;
  Common::Ragel::MultiPart body_multi_part_;
  Common::Ragel::Xml body_xml_;
  Common::Ragel::Json body_json_;
  std::string req_body_error_msg_;
  mutable std::optional<std::unordered_multimap<std::string_view, std::string_view>> cookies_;

  // Current evaluation state
private:
  const Engine& engine_;
  RulePhaseType current_phase_{1};
  const Rule* current_rule_{nullptr};
  std::vector<std::string_view> captured_;

  struct TxVariables {
    std::vector<Common::Variant> variables_;
    std::unordered_map<std::string, size_t> local_index_;
    std::unordered_map<size_t, std::string> local_index_reverse_;
  };
  std::unordered_map<std::string /*namespace*/, TxVariables> tx_variables_;

  // Stores all matched variables organized by rule chain index.
  // - Key: rule chain index (-1 for top-level rules, >=0 for chained rules)
  // - Value: vector of all variables that matched within that specific rule
  // Used by MATCHED_VAR, MATCHED_VARS, MATCHED_VAR_NAME, MATCHED_VARS_NAMES variables.
  std::unordered_map<RuleChainIndexType, std::vector<MatchedVariable>> matched_variables_;

  // Stores all matched PTree nodes organized by rule chain index.
  // - Key: rule chain index (-1 for top-level rules, >=0 for chained rules)
  // - Value: vector of all PTree nodes that matched within that specific rule
  // Used by MATCHED_OPTREE and MATCHED_VPTREE variables.
  std::unordered_map<RuleChainIndexType, std::vector<const Common::PropertyTree*>> matched_optrees_;
  std::vector<const Common::PropertyTree*> temp_matched_optrees_;
  std::unordered_map<RuleChainIndexType, std::vector<const Common::PropertyTree*>> matched_vptrees_;

  // All of the transaction instances share the same rule instances, and each transaction instance
  // may be removed or updated some different rules by the ctl action. So, we need to mark the rules
  // that need to be removed or updated in local.
  // The allocation memory behavior is lazy, and only the rules that need to be removed or updated
  // will be allocated memory that is same as the engin.rules() size.
  std::array<std::vector<bool>, PHASE_TOTAL> rule_remove_flags_;
  std::array<std::vector<boost::unordered_flat_set<Variable::FullName>>, PHASE_TOTAL>
      rule_remove_targets_;

  TransformCache transform_cache_;
  std::bitset<PHASE_TOTAL> allow_phases_;

  std::array<std::variant<std::monostate, std::string, const Macro::MacroBase*>,
             static_cast<size_t>(PersistentStorage::Storage::Type::SizeOfType)>
      persistent_storage_keys_;

  // Configuration options by ctl action
private:
  std::optional<AuditLogConfig::AuditEngine> audit_engine_;
  std::optional<AuditLogConfig::AuditLogPart> audit_log_part_;
  std::optional<EngineConfig::Option> request_body_access_;
  std::optional<BodyProcessorType> request_body_processor_;
  std::optional<ParseXmlIntoArgsOption> parse_xml_into_args_;
  std::optional<EngineConfig::Option> rule_engine_;

private:
  mutable std::optional<std::string> unique_id_;
  LogCallback log_callback_;
  void* log_user_data_;
  AdditionalCondCallback additional_cond_;
  void* additional_cond_user_data_;
  std::forward_list<std::string> string_pool_;
};

using TransactionPtr = std::unique_ptr<Transaction>;
} // namespace Wge