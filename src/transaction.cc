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
#include "transaction.h"

#include <chrono>
#include <format>

#include "action/set_var.h"
#include "common/assert.h"
#include "common/log.h"
#include "common/ragel/uri_parser.h"
#include "common/string.h"
#include "common/try.h"
#include "engine.h"
#include "variable/variables_include.h"

namespace Wge {
// To avoid the dynamic memory allocation, we allocate the memory for the variable vector in
// advance. We assume that the count of variable that the key of varabile contains macro is less
// than variable_key_with_macro_size.
constexpr size_t variable_key_with_macro_size = 100;
constexpr int max_capture_size = 100;

Transaction::Transaction(const Engine& engin) : engine_(engin) {
  for (auto& [ns, size] : engine_.getTxVariableIndexSize()) {
    auto& tx_var_info = tx_variables_[ns];
    tx_var_info.variables_.reserve(size + variable_key_with_macro_size);
    tx_var_info.variables_.resize(size);
    assert(tx_var_info.variables_.capacity() == size + variable_key_with_macro_size);
    tx_var_info.local_index_.reserve(variable_key_with_macro_size);
    tx_var_info.local_index_reverse_.reserve(variable_key_with_macro_size);
  }

  captured_.reserve(4);
  matched_variables_.reserve(4);
  transform_cache_.reserve(100);
}

void Transaction::processConnection(std::string_view downstream_ip, short downstream_port,
                                    std::string_view upstream_ip, short upstream_port) {
  WGE_LOG_TRACE("====process connection====");
  connection_info_.downstream_ip_ = downstream_ip;
  connection_info_.upstream_ip_ = upstream_ip;
  connection_info_.downstream_port_ = downstream_port;
  connection_info_.upstream_port_ = upstream_port;
}

void Transaction::processUri(std::string_view request_line) {
  request_line_ = request_line;
  // Find the first space to extract the HTTP method
  size_t pos_space1 = request_line.find(' ');
  if (pos_space1 != std::string_view::npos)
    [[likely]] {
      std::string_view method = request_line.substr(0, pos_space1);
      // Find the second space to extract the URI
      size_t pos_space2 = request_line.find(' ', pos_space1 + 1);
      if (pos_space2 != std::string_view::npos)
        [[likely]] {
          std::string_view uri = request_line.substr(pos_space1 + 1, pos_space2 - pos_space1 - 1);
          // Extract the protocol string (e.g., "HTTP/1.1")
          request_line_info_.protocol_ = request_line.substr(pos_space2 + 1);

          // Extract the version part after the '/' in the protocol string
          auto pos = request_line_info_.protocol_.find('/');
          if (pos != std::string_view::npos)
            [[likely]] { processUri(uri, method, request_line_info_.protocol_.substr(pos + 1)); }
        }
    }
}

void Transaction::processUri(std::string_view uri, std::string_view method,
                             std::string_view version) {
  WGE_LOG_TRACE("====process uri====");
  // If request_line_ is empty, reconstruct it using method, URI, and version
  if (request_line_.empty()) {
    std::string request_line_buffer;
    request_line_buffer.reserve(method.size() + uri.size() + version.size() + 7);
    request_line_buffer += method;
    request_line_buffer += ' ';
    request_line_buffer += uri;
    request_line_buffer += " HTTP/";
    request_line_buffer += version;
    request_line_ = internString(std::move(request_line_buffer));
    // Extract protocol string from the reconstructed request line
    request_line_info_.protocol_ = request_line_.substr(method.size() + uri.size() + 2);
  }
  // method
  request_line_info_.method_ = method;

  // uri_raw
  request_line_info_.uri_raw_ = uri;

  // version
  request_line_info_.version_ = version;

  Common::Ragel::UriParser uri_parser;
  uri_parser.init(uri, request_line_info_, string_pool_);

  // Init the query params
  request_line_info_.query_params_.init(request_line_info_.query_, string_pool_);

  WGE_LOG_TRACE("method: {}, uri: {}, query: {}, protocol: {}, version: {}",
                request_line_info_.method_, request_line_info_.uri_, request_line_info_.query_,
                request_line_info_.protocol_, request_line_info_.version_);
}

bool Transaction::processRequestHeaders(HeaderFind request_header_find,
                                        HeaderTraversal request_header_traversal,
                                        size_t request_header_count, LogCallback log_callback,
                                        void* log_user_data, AdditionalCondCallback additional_cond,
                                        void* additional_cond_user_data) {
  WGE_LOG_TRACE("====process request headers====");
  extractor_.request_header_find_ = std::move(request_header_find);
  extractor_.request_header_traversal_ = std::move(request_header_traversal);
  extractor_.request_header_count_ = request_header_count;
  log_callback_ = log_callback;
  log_user_data_ = log_user_data;
  additional_cond_ = additional_cond;
  additional_cond_user_data_ = additional_cond_user_data;

  // Set the request body processor
  if (extractor_.request_header_find_) {
    std::string_view content_type;
    auto results = extractor_.request_header_find_("content-type");
    if (!results.empty()) {
      content_type = results.front();
    }
    if (content_type.starts_with("application/x-www-form-urlencoded")) {
      request_body_processor_ = BodyProcessorType::UrlEncoded;
    } else if (content_type.starts_with("multipart/form-data")) {
      request_body_processor_ = BodyProcessorType::MultiPart;
    } else {
      request_body_processor_ = BodyProcessorType::UnknownFormat;
    }
    // The xml and json processor must be specified by the ctl action.
    // else if (content_type == "application/xml" || content_type == "text/xml") {
    //   request_body_processor_ = BodyProcessorType::Xml;
    // } else if (content_type == "application/json") {
    //   request_body_processor_ = BodyProcessorType::Json;
    // }
  }

  bool result = process(1);

  // Reset the log callback and additional condition
  log_callback_ = nullptr;
  log_user_data_ = nullptr;
  additional_cond_ = nullptr;
  additional_cond_user_data_ = nullptr;

  return result;
}

bool Transaction::processRequestBody(std::string_view body, LogCallback log_callback,
                                     void* log_user_data, AdditionalCondCallback additional_cond,
                                     void* additional_cond_user_data) {
  WGE_LOG_TRACE("====process request body====");
  request_body_ = body;
  log_callback_ = log_callback;
  log_user_data_ = log_user_data;
  additional_cond_ = additional_cond;
  additional_cond_user_data_ = additional_cond_user_data;

  // Parse the query params
  if (!request_body_.empty() && request_body_processor_.has_value()) {
    switch (request_body_processor_.value()) {
    case BodyProcessorType::UnknownFormat: {
      // Do nothing
    } break;
    case BodyProcessorType::UrlEncoded: {
      body_query_param_.init(request_body_, string_pool_);
    } break;
    case BodyProcessorType::MultiPart: {
      std::string_view content_type;
      auto results = extractor_.request_header_find_("content-type");
      if (!results.empty()) {
        content_type = results.front();
      }
      body_multi_part_.init(content_type, request_body_, engine_.config().upload_file_limit_);
    } break;
    case BodyProcessorType::Xml: {
      body_xml_.init(request_body_, string_pool_);
      auto option = getParseXmlIntoArgs();
      if (option != ParseXmlIntoArgsOption::Off) {
        body_query_param_.merge(body_xml_.getTags());
      }
      if (option == ParseXmlIntoArgsOption::OnlyArgs) {
        body_xml_.clear();
      }
    } break;
    case BodyProcessorType::Json: {
      body_json_.init(request_body_, string_pool_);
    } break;
    default: {
      UNREACHABLE();
    } break;
    }
  }

  bool result = process(2);

  // Reset the log callback and additional condition
  log_callback_ = nullptr;
  log_user_data_ = nullptr;
  additional_cond_ = nullptr;
  additional_cond_user_data_ = nullptr;

  return result;
}

bool Transaction::processResponseHeaders(std::string_view status_code, std::string_view protocol,
                                         HeaderFind response_header_find,
                                         HeaderTraversal response_header_traversal,
                                         size_t response_header_count, LogCallback log_callback,
                                         void* log_user_data,
                                         AdditionalCondCallback additional_cond,
                                         void* additional_cond_user_data) {
  WGE_LOG_TRACE("====process response headers====");
  extractor_.response_header_find_ = std::move(response_header_find);
  extractor_.response_header_traversal_ = std::move(response_header_traversal);
  extractor_.response_header_count_ = response_header_count;
  response_line_info_.status_code_ = status_code;
  response_line_info_.protocol_ = protocol;

  log_callback_ = log_callback;
  log_user_data_ = log_user_data;
  additional_cond_ = additional_cond;
  additional_cond_user_data_ = additional_cond_user_data;

  bool result = process(3);

  // Reset the log callback and additional condition
  log_callback_ = nullptr;
  log_user_data_ = nullptr;
  additional_cond_ = nullptr;
  additional_cond_user_data_ = nullptr;

  return result;
}

bool Transaction::processResponseBody(std::string_view body, LogCallback log_callback,
                                      void* log_user_data, AdditionalCondCallback additional_cond,
                                      void* additional_cond_user_data) {
  WGE_LOG_TRACE("====process response body====");
  response_body_ = body;
  log_callback_ = log_callback;
  log_user_data_ = log_user_data;
  additional_cond_ = additional_cond;
  additional_cond_user_data_ = additional_cond_user_data;

  bool result = process(4);

  // Reset the log callback and additional condition
  log_callback_ = nullptr;
  log_user_data_ = nullptr;
  additional_cond_ = nullptr;
  additional_cond_user_data_ = nullptr;

  return result;
}

void Transaction::setVariable(const std::string& ns, size_t index, const Common::Variant& value) {
  auto iter_ns = tx_variables_.find(ns);
  if (iter_ns != tx_variables_.end()) {
    auto& variables = iter_ns->second.variables_;
    assert(index < variables.size());
    if (index < variables.size()) {
      assert(!IS_EMPTY_VARIANT(value));
      variables[index] = value;
    }
  }
}

void Transaction::setVariable(const std::string& ns, std::string&& name,
                              const Common::Variant& value) {
  auto index = engine_.getTxVariableIndex(ns, name);
  if (index.has_value())
    [[likely]] { setVariable(ns, index.value(), value); }
  else {
    auto local_index = getOrCreateLocalVariableIndex(ns, name);
    setVariable(ns, local_index, value);
  }
}

void Transaction::removeVariable(const std::string& ns, size_t index) {
  auto iter_ns = tx_variables_.find(ns);
  if (iter_ns != tx_variables_.end()) {
    auto& variables = iter_ns->second.variables_;
    assert(index < variables.size());
    if (index < variables.size()) {
      assert(!IS_EMPTY_VARIANT(variables[index]));
      variables[index] = EMPTY_VARIANT;
    }
  }
}

void Transaction::removeVariable(const std::string& ns, const std::string& name) {
  auto index = engine_.getTxVariableIndex(ns, name);
  if (index.has_value()) {
    removeVariable(ns, index.value());
  } else {
    auto local_index = getLocalVariableIndex(ns, name);
    assert(local_index.has_value());
    if (local_index.has_value())
      [[likely]] { removeVariable(ns, local_index.value()); }
  }
}

void Transaction::increaseVariable(const std::string& ns, size_t index, int64_t value) {
  auto iter_ns = tx_variables_.find(ns);
  if (iter_ns != tx_variables_.end()) {
    auto& variables = iter_ns->second.variables_;
    assert(index < variables.size());
    if (index < variables.size()) {
      auto& variant = variables[index];
      if (IS_INT_VARIANT(variant))
        [[likely]] { variant = std::get<int64_t>(variant) + value; }
      else if (IS_EMPTY_VARIANT(variant)) {
        variant = value;
      }
    }
  }
}

void Transaction::increaseVariable(const std::string& ns, const std::string& name, int64_t value) {
  auto index = engine_.getTxVariableIndex(ns, name);
  if (index.has_value()) {
    increaseVariable(ns, index.value(), value);
  } else {
    auto local_index = getOrCreateLocalVariableIndex(ns, name);
    increaseVariable(ns, local_index, value);
  }
}

const Common::Variant& Transaction::getVariable(const std::string& ns, size_t index) const {
  auto iter_ns = tx_variables_.find(ns);
  if (iter_ns != tx_variables_.end()) {
    auto& variables = iter_ns->second.variables_;
    assert(index < variables.size());
    if (index < variables.size()) {
      return variables[index];
    }
  }

  return EMPTY_VARIANT;
}

const Common::Variant& Transaction::getVariable(const std::string& ns,
                                                const std::string& name) const {
  auto index = engine_.getTxVariableIndex(ns, name);
  if (index.has_value()) {
    return getVariable(ns, index.value());
  } else {
    auto local_index = getLocalVariableIndex(ns, name);
    assert(local_index.has_value());
    if (local_index.has_value())
      [[likely]] { return getVariable(ns, local_index.value()); }
  }

  return EMPTY_VARIANT;
}

std::vector<std::pair<std::string_view, const Common::Variant*>>
Transaction::getVariables(const std::string& ns) const {
  std::vector<std::pair<std::string_view, const Common::Variant*>> results;
  auto iter_ns = tx_variables_.find(ns);
  if (iter_ns == tx_variables_.end())
    [[unlikely]] { return results; }

  size_t literal_key_size = 0;
  auto iter_size = engine_.getTxVariableIndexSize().find(ns);
  if (iter_size != engine_.getTxVariableIndexSize().end()) {
    literal_key_size = iter_size->second;
  }

  const auto& variables = iter_ns->second.variables_;
  results.reserve(variables.size());
  for (size_t i = 0; i < variables.size(); ++i) {
    const auto& variable = variables[i];
    if (!IS_EMPTY_VARIANT(variable)) {
      if (i < literal_key_size) {
        results.emplace_back(engine_.getTxVariableIndexReverse(ns, i), &variable);
      } else {
        auto iter = iter_ns->second.local_index_reverse_.find(i);
        if (iter != iter_ns->second.local_index_reverse_.end()) {
          results.emplace_back(iter->second, &variable);
        }
      }
    }
  }
  return results;
}

int64_t Transaction::getVariablesCount(const std::string& ns) const {
  int64_t count = 0;
  auto iter_ns = tx_variables_.find(ns);
  if (iter_ns != tx_variables_.end()) {
    for (auto& variable : iter_ns->second.variables_) {
      if (!IS_EMPTY_VARIANT(variable)) {
        ++count;
      }
    }
  }
  return count;
}

bool Transaction::hasVariable(const std::string& ns, size_t index) const {
  auto iter_ns = tx_variables_.find(ns);
  if (iter_ns == tx_variables_.end())
    [[unlikely]] { return false; }

  assert(index < iter_ns->second.variables_.size());
  return index < iter_ns->second.variables_.size() &&
         !IS_EMPTY_VARIANT(iter_ns->second.variables_[index]);
}

bool Transaction::hasVariable(const std::string& ns, const std::string& name) const {
  auto index = engine_.getTxVariableIndex(ns, name);
  if (index.has_value())
    [[likely]] { return index.has_value() && hasVariable(ns, index.value()); }
  else {
    auto local_index = getLocalVariableIndex(ns, name);
    assert(local_index.has_value());
    return local_index.has_value() && hasVariable(ns, local_index.value());
  }
}

void Transaction::setCapture(size_t index, std::string_view value) {
  if (index < max_capture_size)
    [[likely]] {
      if (captured_.size() <= index) {
        captured_.resize(index + 1);
      }
      captured_[index] = value;
    }
}

std::string_view Transaction::getCapture(size_t index) const {
  // assert(index < matched_size_);
  if (index < captured_.size())
    [[likely]] { return captured_[index]; }
  else {
    WGE_LOG_WARN("The index of captured string is out of range. index: {}, captured size: {}",
                 index, captured_.size());
    return "";
  }
}

void Transaction::pushMatchedVariable(
    const Variable::VariableBase* variable, RuleChainIndexType rule_chain_index,
    const Common::EvaluateElement& original_value, const Common::EvaluateElement& transformed_value,
    std::string_view captured_value,
    std::list<const Transformation::TransformBase*>&& transform_list) {
  auto& variables = matched_variables_.try_emplace(rule_chain_index, std::vector<MatchedVariable>())
                        .first->second;

  variables.emplace_back(variable, original_value, transformed_value, captured_value,
                         std::move(transform_list));

  if (IS_EMPTY_VARIANT(variables.back().transformed_value_.variant_))
    [[unlikely]] {
      auto& transform_value = variables.back().transformed_value_;
      auto& original_value = variables.back().original_value_;
      transform_value.variant_ = original_value.variant_;
      transform_value.variable_sub_name_ = original_value.variable_sub_name_;
    }
}

void Transaction::removeRule(
    const std::array<std::unordered_set<const Rule*>, PHASE_TOTAL>& rules) {
  // Sets the rule remove flags
  for (RulePhaseType phase = current_phase_; phase < PHASE_TOTAL; ++phase) {
    auto& rule_set = rules[phase - 1];
    if (rule_set.empty())
      [[likely]] { continue; }

    // For performance reasons, we use a flag array that is the same size as the rules array to
    // mark the rules that need to be removed.
    auto& rule_remove_flag = rule_remove_flags_[phase - 1];
    if (rule_remove_flag.empty())
      [[unlikely]] { rule_remove_flag.resize(engine_.rules(phase).size()); }

    // Mark the rules as removed
    for (auto& rule : rule_set) {
      assert(rule->index() != -1);
      rule_remove_flag[rule->index()] = true;
    }
  }
}

void Transaction::removeRuleTarget(
    const std::array<std::unordered_set<const Rule*>, PHASE_TOTAL>& rules,
    const std::vector<std::shared_ptr<Variable::VariableBase>>& variables) {
  // Sets the rule remove targets
  for (RulePhaseType phase = current_phase_; phase < PHASE_TOTAL; ++phase) {
    auto& rule_set = rules[phase - 1];
    if (rule_set.empty())
      [[likely]] { continue; }

    // For performance reasons, we use a flag array that is the same size as the rules array to
    // mark the variables of the rules that need to be removed.
    auto& rule_remove_targets = rule_remove_targets_[phase - 1];
    if (rule_remove_targets.empty())
      [[unlikely]] { rule_remove_targets.resize(engine_.rules(phase).size()); }

    // Mark the variables of the rules as removed
    for (auto& rule : rule_set) {
      for (auto& variable : variables) {
        assert(rule->index() != -1);
        rule_remove_targets[rule->index()].insert(variable->fullName());
      }
    }
  }
}

bool Transaction::isRuleTargetRemoved(const Rule* rule, Variable::FullName full_name) const {
  auto& rule_remove_targets = rule_remove_targets_[rule->phase() - 1];
  if (rule_remove_targets.empty())
    [[unlikely]] { return false; }

  assert(rule->index() != -1);
  auto& remove_variables = rule_remove_targets[rule->index()];
  if (!remove_variables.empty())
    [[unlikely]] {
      auto iter = remove_variables.find(full_name);
      if (iter != remove_variables.end())
        [[unlikely]] { return true; }
      else {
        // May be the ctl action remove the variable by the main name only, that means remove the
        // whole collection
        if (!full_name.sub_name_.empty()) {
          iter = remove_variables.find({full_name.main_name_, ""});
          if (iter != remove_variables.end())
            [[unlikely]] { return true; }
        }
      }
    }

  return false;
}

std::string_view Transaction::getMsgMacroExpanded() {
  if (current_rule_ && current_rule_->msgMacro()) {
    Wge::Common::EvaluateResults result;
    current_rule_->msgMacro()->evaluate(*this, result);
    WGE_LOG_TRACE("evaluate msg macro: {}", VISTIT_VARIANT_AS_STRING(result.at(0).variant_));
    if (IS_STRING_VIEW_VARIANT(result.front().variant_)) {
      return std::get<std::string_view>(result.front().variant_);
    }
  }

  return "";
}

std::string_view Transaction::getLogDataMacroExpanded() {
  if (current_rule_ && current_rule_->logDataMacro()) {
    Wge::Common::EvaluateResults result;
    current_rule_->logDataMacro()->evaluate(*this, result);
    WGE_LOG_TRACE("evaluate logdata macro: {}", VISTIT_VARIANT_AS_STRING(result.at(0).variant_));
    if (IS_STRING_VIEW_VARIANT(result.front().variant_)) {
      return std::get<std::string_view>(result.front().variant_);
    }
  }

  return "";
}

std::string_view Transaction::getPersistentStorageKey(PersistentStorage::Storage::Type type) const {
  switch (persistent_storage_keys_[static_cast<size_t>(type)].index()) {
  case 1:
    return std::get<std::string>(persistent_storage_keys_[static_cast<size_t>(type)]);
  case 2: {
    const Macro::MacroBase* macro =
        std::get<const Macro::MacroBase*>(persistent_storage_keys_[static_cast<size_t>(type)]);
    Common::EvaluateResults result;
    macro->evaluate(*const_cast<Transaction*>(this), result);
    if (IS_STRING_VIEW_VARIANT(result.front().variant_)) {
      return std::get<std::string_view>(result.front().variant_);
    }
  }
  default:
    return "";
  }
}

ParseXmlIntoArgsOption Transaction::getParseXmlIntoArgs() const {
  return parse_xml_into_args_.value_or(engine_.parseXmlIntoArgsOption());
}

std::string_view Transaction::getUniqueId() const {
  // We doesn't generate the unique id in the constructor, because the rules may be not use the
  // unique id any more, so we generate the unique id when the unique id is needed.
  // This is a lazy initialization, ant it's will be increased the performance.
  if (!unique_id_) {
    initUniqueId();
  }
  return *unique_id_;
}

void Transaction::initUniqueId() const {
  // Generate a unique id for the transaction.
  // Implementation is to use a millisecond timestamp, followed by a dot character ('.'), followed
  // by a random six-digit number.
  using namespace std::chrono;
  uint64_t timestamp =
      time_point_cast<std::chrono::milliseconds>(system_clock::now()).time_since_epoch().count();
  int random = ::rand() % 100000 + 100000;
  unique_id_ = std::format("{}.{}", timestamp, random);
}

inline bool Transaction::process(RulePhaseType phase) {
  if (engine_.config().rule_engine_option_ == EngineConfig::Option::Off)
    [[unlikely]] { return true; }

  current_phase_ = phase;

  // Skip the phase that is allowed
  if (allow_phases_.test(phase))
    [[unlikely]] { return true; }

  // Get the rules in the given phase
  auto& rules = engine_.rules(phase);
  const Wge::Rule* default_action = engine_.defaultActions(phase);

  // Traverse the rules and evaluate them
  auto begin = rules.begin();
  auto& rule_remove_flag = rule_remove_flags_[phase - 1];
  for (auto iter = begin; iter != rules.end();) {
    current_rule_ = &(*iter);

    // Skip the rules that have been removed
    assert(current_rule_->index() != -1);
    if (!rule_remove_flag.empty() && rule_remove_flag[current_rule_->index()])
      [[unlikely]] {
        ++iter;
        continue;
      }

    // Clean the current captured and matched, there are:
    // TX.[0-99], MATCHED_VAR_NAME, MATCHED_VAR, MATCHED_VARS_NAMES, MATCHED_VARS
    captured_.clear();
    matched_variables_.clear();

    // Evaluate the rule
    auto is_matched = current_rule_->evaluate(*this);

    if (!is_matched || current_rule_->operators().empty())
      [[likely]] {
        ++iter;
        continue;
      }

    // Log the matched rule
    if (log_callback_)
      [[likely]] {
        if (current_rule_->log()) {
          log_callback_(*current_rule_, log_user_data_);
        }
      }

    // Do the disruptive action
    if (engine_.config().rule_engine_option_ != EngineConfig::Option::DetectionOnly) {
      std::optional<bool> disruptive = doDisruptive(*current_rule_, default_action);
      if (disruptive.has_value()) {
        if (!disruptive.value()) {
          // Modify the response status code
          response_line_info_.status_code_ = "403";
        }
        return disruptive.value();
      }
    }

    // Skip the rules if current rule that has a skip action or skipAfter action is matched
    int skip = current_rule_->skip();
    if (skip > 0)
      [[unlikely]] {
        if (std::distance(iter, rules.end()) > skip + 1) {
          iter += skip + 1;
        } else {
          iter = rules.end();
        }
        continue;
      }

    // If skip and skipAfter are not set, then continue to the next rule
    ++iter;
  }

  return true;
}

inline std::optional<size_t> Transaction::getLocalVariableIndex(const std::string& ns,
                                                                const std::string& key) const {
  // The key is case insensitive
  std::string less_case_key;
  less_case_key.reserve(key.size());
  std::transform(key.begin(), key.end(), std::back_inserter(less_case_key), ::tolower);

  auto iter_ns = tx_variables_.find(ns);
  if (iter_ns == tx_variables_.end())
    [[unlikely]] { return std::nullopt; }

  auto iter = iter_ns->second.local_index_.find(less_case_key);
  if (iter == iter_ns->second.local_index_.end())
    [[unlikely]] { return std::nullopt; }

  return iter->second;
}

inline size_t Transaction::getOrCreateLocalVariableIndex(const std::string& ns,
                                                         const std::string& key) {

  // The key is case insensitive
  std::string less_case_key;
  less_case_key.reserve(key.size());
  std::transform(key.begin(), key.end(), std::back_inserter(less_case_key), ::tolower);

  auto iter_ns = tx_variables_.find(ns);
  if (iter_ns == tx_variables_.end())
    [[unlikely]] {
      auto inserted_iter = tx_variables_.emplace(ns, TxVariables{}).first;
      inserted_iter->second.local_index_.insert({less_case_key, 0});
      inserted_iter->second.local_index_reverse_.insert({0, less_case_key});
      inserted_iter->second.variables_.emplace_back();
      return 0;
    }

  auto iter = iter_ns->second.local_index_.find(less_case_key);
  if (iter == iter_ns->second.local_index_.end())
    [[unlikely]] {
      auto& variables = iter_ns->second.variables_;
      iter_ns->second.local_index_.insert({less_case_key, variables.size()});
      iter_ns->second.local_index_reverse_.insert({variables.size(), less_case_key});
      variables.emplace_back();
      return variables.size() - 1;
    }

  return iter->second;
}

void Transaction::initCookies() const {
  if (cookies_.has_value())
    [[likely]] { return; }

  cookies_.emplace();

  // Get the cookies form the request headers
  std::vector<std::string_view> result = extractor_.request_header_find_("cookie");

  // Parse the cookies
  for (auto& cookies : result) {
    size_t begin = 0;
    size_t end = 0;
    while (end != std::string_view::npos) {
      end = cookies.find(';', begin);
      auto cookie = cookies.substr(begin, end - begin);
      auto pos = cookie.find('=');
      if (pos != std::string_view::npos) {
        std::string_view key = Common::trim(cookie.substr(0, pos));
        std::string_view value = Common::trim(cookie.substr(pos + 1));
        cookies_->emplace(key, value);
      }
      begin = end + 1;
    }
  }
}

inline std::optional<bool> Transaction::doDisruptive(const Rule& rule, const Rule* default_action) {
  switch (rule.disruptive()) {
  case Rule::Disruptive::ALLOW: {
    // If used on its own, allow will affect the entire transaction, stopping processing of the
    // current phase but also skipping over all other phases apart from the logging phase. (The
    // logging phase is special; it is designed to always execute.)
    allow_phases_.set(1);
    allow_phases_.set(2);
    allow_phases_.set(3);
    allow_phases_.set(4);
    return true;
  } break;
  case Rule::Disruptive::ALLOW_PHASE: {
    // If used with parameter "phase", allow will cause the engine to stop processing the current
    // phase. Other phases will continue as normal.
    allow_phases_.set(rule.phase());
    return true;
  } break;
  case Rule::Disruptive::ALLOW_REQUEST: {
    // If used with parameter "request", allow will cause the engine to stop processing the current
    // phase. The next phase to be processed will be phase RESPONSE_HEADERS.
    allow_phases_.set(1);
    allow_phases_.set(2);
    return true;
  } break;
    [[likely]] case Rule::Disruptive::BLOCK : {
      // Performs the disruptive action defined by the previous SecDefaultAction.
      Rule::Disruptive disruptive =
          default_action ? default_action->disruptive() : Rule::Disruptive::PASS;
      switch (disruptive) {
      case Rule::Disruptive::ALLOW: {
        // If used on its own, allow will affect the entire transaction, stopping processing of the
        // current phase but also skipping over all other phases apart from the logging phase. (The
        // logging phase is special; it is designed to always execute.)
        allow_phases_.set(1);
        allow_phases_.set(2);
        allow_phases_.set(3);
        allow_phases_.set(4);
        return true;
      } break;
      case Rule::Disruptive::ALLOW_PHASE: {
        // If used with parameter "phase", allow will cause the engine to stop processing the
        // current phase. Other phases will continue as normal.
        allow_phases_.set(rule.phase());
        return true;
      } break;
      case Rule::Disruptive::ALLOW_REQUEST: {
        // If used with parameter "request", allow will cause the engine to stop processing the
        // current phase. The next phase to be processed will be phase RESPONSE_HEADERS.
        allow_phases_.set(1);
        allow_phases_.set(2);
        return true;
      } break;
      case Rule::Disruptive::BLOCK: {
        // Performs the disruptive action defined by the previous SecDefaultAction.
        // We do nothing here, and continue to the next rule.
      } break;
      case Rule::Disruptive::DENY:
      case Rule::Disruptive::DROP: {
        // Stops rule processing and intercepts transaction.
        return false;
      } break;
      case Rule::Disruptive::PASS: {
        // Continues processing with the next rule in spite of a successful match.
        // We do nothing here, and continue to the next rule.
      } break;
      case Rule::Disruptive::REDIRECT: {
        // Intercepts transaction by issuing an external (client-visible) redirection to the given
        // location..
        // FIXME(zhouyu 2025-03-28): implement the redirect action
        UNREACHABLE();
      } break;
      default:
        UNREACHABLE();
        break;
      }
    }
    break;
  case Rule::Disruptive::DENY:
  case Rule::Disruptive::DROP: {
    // Stops rule processing and intercepts transaction.
    return false;
  } break;
  case Rule::Disruptive::PASS: {
    // Continues processing with the next rule in spite of a successful match.
    // We do nothing here, and continue to the next rule.
  } break;
  case Rule::Disruptive::REDIRECT: {
    // Intercepts transaction by issuing an external (client-visible) redirection to the given
    // location..
    // FIXME(zhouyu 2025-03-28): implement the redirect action
    UNREACHABLE();
  } break;
  default:
    UNREACHABLE();
    break;
  }

  return std::nullopt;
}
} // namespace Wge