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
#include "common/empty_string.h"
#include "common/log.h"
#include "common/ragel/uri_parser.h"
#include "common/try.h"
#include "engine.h"
#include "variable/variables_include.h"

namespace Wge {
const Transaction::RandomInitHelper Transaction::random_init_helper_;

// To avoid the dynamic memory allocation, we allocate the memory for the variable vector in
// advance. We assume that the count of variable that the key of varabile contains macro is less
// than variable_key_with_macro_size.
constexpr size_t variable_key_with_macro_size = 100;

Transaction::Transaction(const Engine& engin, size_t literal_key_size)
    : engine_(engin), tx_variables_(literal_key_size + variable_key_with_macro_size),
      literal_key_size_(literal_key_size) {
  tx_variables_.resize(literal_key_size);
  local_tx_variable_index_.reserve(variable_key_with_macro_size);
  local_tx_variable_index_reverse_.reserve(variable_key_with_macro_size);
  captured_.reserve(4);
  matched_variables_.reserve(4);
  transform_cache_.reserve(100);
  assert(tx_variables_.capacity() == literal_key_size + variable_key_with_macro_size);
}

void Transaction::processConnection(std::string_view downstream_ip, short downstream_port,
                                    std::string_view upstream_ip, short upstream_port) {
  WGE_LOG_TRACE("====process connection====");
  connection_info_.downstream_ip_ = downstream_ip;
  connection_info_.downstream_port_ = downstream_port;
  connection_info_.upstream_ip_ = upstream_ip;
  connection_info_.upstream_port_ = upstream_port;
}

void Transaction::processUri(std::string_view request_line) {
  request_line_ = request_line;
  // Find the first space to extract the HTTP method
  size_t pos_space1 = request_line.find(' ');
  if (pos_space1 != std::string_view::npos) [[likely]] {
    std::string_view method = request_line.substr(0, pos_space1);
    // Find the second space to extract the URI
    size_t pos_space2 = request_line.find(' ', pos_space1 + 1);
    if (pos_space2 != std::string_view::npos) [[likely]] {
      std::string_view uri = request_line.substr(pos_space1 + 1, pos_space2 - pos_space1 - 1);
      // Extract the protocol string (e.g., "HTTP/1.1")
      request_line_info_.protocol_ = request_line.substr(pos_space2 + 1);

      // Extract the version part after the '/' in the protocol string
      auto pos = request_line_info_.protocol_.find('/');
      if (pos != std::string_view::npos) [[likely]] {
        processUri(uri, method, request_line_info_.protocol_.substr(pos + 1));
      }
    }
  }
}

void Transaction::processUri(std::string_view uri, std::string_view method,
                             std::string_view version) {
  WGE_LOG_TRACE("====process uri====");
  // If request_line_ is empty, reconstruct it using method, URI, and version
  if (request_line_.empty()) {
    request_line_buffer_.reserve(method.size() + uri.size() + version.size() + 7);
    request_line_buffer_ += method;
    request_line_buffer_ += ' ';
    request_line_buffer_ += uri;
    request_line_buffer_ += " HTTP/";
    request_line_buffer_ += version;
    request_line_ = request_line_buffer_;
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
  uri_parser.init(uri, request_line_info_);

  // Init the query params
  request_line_info_.query_params_.init(request_line_info_.query_);

  WGE_LOG_TRACE("method: {}, uri: {}, query: {}, protocol: {}, version: {}",
                request_line_info_.method_, request_line_info_.uri_, request_line_info_.query_,
                request_line_info_.protocol_, request_line_info_.version_);
}

bool Transaction::processRequestHeaders(HeaderFind request_header_find,
                                        HeaderTraversal request_header_traversal,
                                        size_t header_count,
                                        std::function<void(const Rule&)> log_callback) {
  WGE_LOG_TRACE("====process request headers====");
  extractor_.request_header_find_ = std::move(request_header_find);
  extractor_.request_header_traversal_ = std::move(request_header_traversal);
  extractor_.request_header_count_ = header_count;
  log_callback_ = std::move(log_callback);

  // Set the request body processor
  if (extractor_.request_header_find_) {
    auto content_type = extractor_.request_header_find_("content-type");
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

  return process(1);
}

bool Transaction::processRequestBody(std::string_view body,
                                     std::function<void(const Rule&)> log_callback) {
  WGE_LOG_TRACE("====process request body====");
  request_body_ = body;
  log_callback_ = std::move(log_callback);

  // Parse the query params
  if (!request_body_.empty() && request_body_processor_.has_value()) {
    switch (request_body_processor_.value()) {
    case BodyProcessorType::UnknownFormat: {
      // Do nothing
    } break;
    case BodyProcessorType::UrlEncoded: {
      body_query_param_.init(request_body_);
    } break;
    case BodyProcessorType::MultiPart: {
      auto content_type = extractor_.request_header_find_("content-type");
      body_multi_part_.init(content_type, request_body_, engine_.config().upload_file_limit_);
    } break;
    case BodyProcessorType::Xml: {
      body_xml_.init(request_body_);
    } break;
    case BodyProcessorType::Json: {
      body_json_.init(request_body_);
    } break;
    default: {
      UNREACHABLE();
    } break;
    }
  }

  return process(2);
}

bool Transaction::processResponseHeaders(std::string_view status_code, std::string_view protocol,
                                         HeaderFind response_header_find,
                                         HeaderTraversal response_header_traversal,
                                         size_t response_header_count,
                                         std::function<void(const Rule&)> log_callback) {
  WGE_LOG_TRACE("====process response headers====");
  extractor_.response_header_find_ = std::move(response_header_find);
  extractor_.response_header_traversal_ = std::move(response_header_traversal);
  extractor_.response_header_count_ = response_header_count;
  log_callback_ = std::move(log_callback);
  response_line_info_.status_code_ = status_code;
  response_line_info_.protocol_ = protocol;
  return process(3);
}

bool Transaction::processResponseBody(std::string_view body,
                                      std::function<void(const Rule&)> log_callback) {
  WGE_LOG_TRACE("====process response body====");
  response_body_ = body;
  log_callback_ = std::move(log_callback);
  return process(4);
}

void Transaction::setVariable(size_t index, const Common::Variant& value) {
  assert(index < tx_variables_.size());
  if (index < tx_variables_.size()) {
    auto& tx_variable = tx_variables_[index];
    if (IS_INT_VARIANT(value)) [[likely]] {
      tx_variable.variant_ = std::get<int>(value);
    } else if (IS_STRING_VIEW_VARIANT(value)) {
      // The tx_variables_ store the value as a variant(std::string_view), and it will be invalid
      // if it's reference is invalid. So we copy the value to the string_buffer_. The
      // string_buffer_ store the value as a string, and it will be valid until the
      // transaction is destroyed. Why we don't let the Common::Variant store the value as a
      // std::string? If we do it, it seems that we don't need the string_buffer_ anymore. But
      // it will cause code diffcult to maintain. Because the Common::Variant is a variant of
      // std::monostate, int, std::string_view. If we add a new std::sting type, we must process
      // the variant in repeat places when we want to get the value as a string. So we choose to
      // store the value as a std::string_view in the Common::Variant, and copy the value to the
      // string_buffer_ when we want to store the value as a string. It's a trade-off between
      // the code maintainability and the performance.
      tx_variable.string_buffer_ = std::get<std::string_view>(value);
      tx_variable.variant_ = tx_variable.string_buffer_;
    } else {
      UNREACHABLE();
    }
  }
}

void Transaction::setVariable(std::string&& name, const Common::Variant& value) {
  auto index = engine_.getTxVariableIndex(name);
  if (index.has_value()) [[likely]] {
    setVariable(index.value(), value);
  } else {
    auto local_index = getLocalVariableIndex(name, true);
    assert(local_index.has_value());
    if (local_index.has_value()) [[likely]] {
      setVariable(local_index.value(), value);
    }
  }
}

void Transaction::removeVariable(size_t index) {
  assert(index < tx_variables_.size());
  if (index < tx_variables_.size()) {
    assert(!IS_EMPTY_VARIANT(tx_variables_[index].variant_));
    tx_variables_[index].variant_ = EMPTY_VARIANT;
  }
}

void Transaction::removeVariable(const std::string& name) {
  auto index = engine_.getTxVariableIndex(name);
  if (index.has_value()) {
    removeVariable(index.value());
  } else {
    auto local_index = getLocalVariableIndex(name, false);
    assert(local_index.has_value());
    if (local_index.has_value()) [[likely]] {
      removeVariable(local_index.value());
    }
  }
}

void Transaction::increaseVariable(size_t index, int value) {
  assert(index < tx_variables_.size());
  if (index < tx_variables_.size()) {
    auto& variant = tx_variables_[index].variant_;
    if (IS_INT_VARIANT(variant)) [[likely]] {
      variant = std::get<int>(variant) + value;
    } else if (IS_EMPTY_VARIANT(variant)) {
      variant = value;
    }
  }
}

void Transaction::increaseVariable(const std::string& name, int value) {
  auto index = engine_.getTxVariableIndex(name);
  if (index.has_value()) {
    increaseVariable(index.value(), value);
  } else {
    auto local_index = getLocalVariableIndex(name, true);
    assert(local_index.has_value());
    if (local_index.has_value()) [[likely]] {
      increaseVariable(local_index.value(), value);
    }
  }
}

const Common::Variant& Transaction::getVariable(size_t index) const {
  assert(index < tx_variables_.size());
  if (index < tx_variables_.size()) {
    return tx_variables_[index].variant_;
  }

  return EMPTY_VARIANT;
}

const Common::Variant& Transaction::getVariable(const std::string& name) {
  auto index = engine_.getTxVariableIndex(name);
  if (index.has_value()) {
    return getVariable(index.value());
  } else {
    auto local_index = getLocalVariableIndex(name, false);
    assert(local_index.has_value());
    if (local_index.has_value()) [[likely]] {
      return getVariable(local_index.value());
    }
  }

  return EMPTY_VARIANT;
}

std::vector<std::pair<std::string_view, Common::Variant*>> Transaction::getVariables() {
  std::vector<std::pair<std::string_view, Common::Variant*>> variables;
  variables.reserve(tx_variables_.size());
  for (size_t i = 0; i < tx_variables_.size(); ++i) {
    auto& variable = tx_variables_[i];
    if (!IS_EMPTY_VARIANT(variable.variant_)) {
      if (i < literal_key_size_) {
        variables.emplace_back(engine_.getTxVariableIndexReverse(i), &variable.variant_);
      } else {
        auto iter = local_tx_variable_index_reverse_.find(i);
        if (iter != local_tx_variable_index_reverse_.end()) {
          variables.emplace_back(iter->second, &variable.variant_);
        }
      }
    }
  }
  return variables;
}

int Transaction::getVariablesCount() const {
  int count = 0;
  for (auto& variable : tx_variables_) {
    if (!IS_EMPTY_VARIANT(variable.variant_)) {
      ++count;
    }
  }
  return count;
}

bool Transaction::hasVariable(size_t index) const {
  assert(index < tx_variables_.size());
  return index < tx_variables_.size() && !IS_EMPTY_VARIANT(tx_variables_[index].variant_);
}

bool Transaction::hasVariable(const std::string& name) const {
  auto index = engine_.getTxVariableIndex(name);
  assert(index.has_value());
  return index.has_value() && hasVariable(index.value());
}

void Transaction::setCapture(size_t index, Common::EvaluateResults::Element&& value) {
  if (index < max_capture_size_) [[likely]] {
    if (captured_.size() <= index) {
      captured_.resize(index + 1);
    }
    captured_[index] = std::move(value);
  }
}

const Common::Variant& Transaction::getCapture(size_t index) const {
  // assert(index < matched_size_);
  if (index < captured_.size()) [[likely]] {
    return captured_[index].variant_;
  } else {
    WGE_LOG_WARN("The index of captured string is out of range. index: {}, captured size: {}",
                 index, captured_.size());
    return EMPTY_VARIANT;
  }
}

const std::string_view Transaction::getUniqueId() {
  // We doesn't generate the unique id in the constructor, because the rules may be not use the
  // unique id any more, so we generate the unique id when the unique id is needed.
  // This is a lazy initialization, ant it's will be increased the performance.
  if (unique_id_.empty()) {
    initUniqueId();
  }
  return unique_id_;
}

void Transaction::removeRule(
    const std::array<std::unordered_set<const Rule*>, PHASE_TOTAL>& rules) {
  // Sets the rule remove flags.
  // Just remove the rules that not evaluate yet. It makes no sense to remove the rules that
  // havebeen evaluated.
  for (size_t phase = current_phase_; phase < PHASE_TOTAL; ++phase) {
    auto& rule_set = rules[phase - 1];
    if (rule_set.empty()) [[likely]] {
      continue;
    }

    // For performance reasons, we use a flag array that is the same size as the rules array to
    // mark the rules that need to be removed.
    auto& rule_remove_flag = rule_remove_flags_[phase - 1];
    if (rule_remove_flag.empty()) [[unlikely]] {
      rule_remove_flag.resize(engine_.rules(phase).size());
    }

    // We record the current rule index, make sure that the rules that have been evaluated will
    // not be removed. As above, it makes no sense to remove the rules that have been evaluated.
    auto& rules = engine_.rules(phase);
    auto begin = rules.begin();
    if (phase == current_phase_) {
      begin += current_rule_index_ + 1;
    }

    // Traverse the rules and mark the rules that need to be removed
    for (auto iter = begin; iter != rules.end(); ++iter) {
      if (rule_set.find(*iter) != rule_set.end()) {
        rule_remove_flag[std::distance(rules.begin(), iter)] = true;
      }
    }
  }
}

void Transaction::removeRuleTarget(
    const std::array<std::unordered_set<const Rule*>, PHASE_TOTAL>& rules,
    const std::vector<std::shared_ptr<Variable::VariableBase>>& variables) {}

void Transaction::pushMatchedVariable(
    const Variable::VariableBase* variable, Common::EvaluateResults::Element&& original_value,
    Common::EvaluateResults::Element&& transformed_value,
    std::vector<const Transformation::TransformBase*>&& transform_list) {
  // Fixes #27
  // When the MATCHED_VARS, MATCHED_VARS_NAMES, MATCHED_VAR,MATCHED_VAR_NAME  are evaluated, the
  // operators should not automatically store the matched variables again.
  std::string_view var_main_name = variable->mainName();
  if (var_main_name == Variable::MatchedVars::main_name_ ||
      var_main_name == Variable::MatchedVarsNames::main_name_ ||
      var_main_name == Variable::MatchedVar::main_name_ ||
      var_main_name == Variable::MatchedVarName::main_name_) [[unlikely]] {
    return;
  }

  matched_variables_.emplace_back(variable, std::move(original_value), std::move(transformed_value),
                                  std::move(transform_list));
  if (IS_EMPTY_VARIANT(matched_variables_.back().transformed_value_.variant_)) [[unlikely]] {
    matched_variables_.back().transformed_value_.variant_ =
        matched_variables_.back().original_value_.variant_;
  }
}

void Transaction::initUniqueId() {
  // Generate a unique id for the transaction.
  // Implementation is to use a millisecond timestamp, followed by a dot character ('.'), followed
  // by a random six-digit number.
  using namespace std::chrono;
  uint64_t timestamp =
      time_point_cast<std::chrono::milliseconds>(system_clock::now()).time_since_epoch().count();
  int random = ::rand() % 100000 + 100000;
  unique_id_ = std::format("{}.{}", timestamp, random);
}

inline bool Transaction::process(int phase) {
  if (engine_.config().rule_engine_option_ == EngineConfig::Option::Off) [[unlikely]] {
    return true;
  }

  current_phase_ = phase;

  // Skip the phase that is allowed
  if (allow_phases_.test(phase)) [[unlikely]] {
    return true;
  }

  // Get the rules in the given phase
  auto& rules = engine_.rules(phase);
  const Wge::Rule* default_action = engine_.defaultActions(phase);

  // Traverse the rules and evaluate them
  auto begin = rules.begin();
  auto& rule_remove_flag = rule_remove_flags_[phase - 1];
  for (auto iter = begin; iter != rules.end();) {
    current_rule_index_ = std::distance(begin, iter);
    current_rule_ = *iter;

    // Skip the rules that have been removed
    if (!rule_remove_flag.empty() && rule_remove_flag[current_rule_index_]) [[unlikely]] {
      ++iter;
      continue;
    }

    // Clean the current captured and matched, there are:
    // TX.[0-99], MATCHED_VAR_NAME, MATCHED_VAR, MATCHED_VARS_NAMES, MATCHED_VARS
    captured_.clear();
    matched_variables_.clear();

    // Evaluate the rule
    auto is_matched = current_rule_->evaluate(*this);

    if (!is_matched ||
        current_rule_->getOperator() == nullptr // It's a rule that defined by SecAction
        ) [[likely]] {
      ++iter;
      continue;
    }

    // Log the matched rule
    if (log_callback_) [[likely]] {
      if (default_action) {
        if (current_rule_->log().value_or(default_action->log().value_or(true))) {
          log_callback_(*current_rule_);
        }
      } else {
        if (current_rule_->log().value_or(true)) {
          log_callback_(*current_rule_);
        }
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
    if (skip > 0) [[unlikely]] {
      iter += skip;
      continue;
    }
    const std::string& skip_after = current_rule_->skipAfter();
    if (!skip_after.empty()) [[unlikely]] {
      auto next_rule_iter = engine_.marker(skip_after, current_rule_->phase());
      if (next_rule_iter.has_value()) [[likely]] {
        iter = next_rule_iter.value();
        continue;
      }
    }

    // If skip and skipAfter are not set, then continue to the next rule
    ++iter;
  }

  return true;
}

inline std::optional<size_t> Transaction::getLocalVariableIndex(const std::string& key,
                                                                bool force_create) {
  // The key is case insensitive
  std::string less_case_key;
  less_case_key.reserve(key.size());
  std::transform(key.begin(), key.end(), std::back_inserter(less_case_key), ::tolower);

  auto iter = local_tx_variable_index_.find(less_case_key);
  if (iter == local_tx_variable_index_.end()) [[unlikely]] {
    if (force_create) [[likely]] {
      local_tx_variable_index_.insert({less_case_key, tx_variables_.size()});
      local_tx_variable_index_reverse_.insert({tx_variables_.size(), less_case_key});
      tx_variables_.emplace_back();
      return tx_variables_.size() - 1;
    } else {
      return std::nullopt;
    }
  }

  return iter->second;
}

void Transaction::initCookies() {
  if (init_cookies_) [[likely]] {
    return;
  }

  init_cookies_ = true;

  // Get the cookies form the request headers
  std::string_view cookies = extractor_.request_header_find_("cookie");

  // Parse the cookies
  size_t begin = 0;
  size_t end = 0;
  while (end != std::string_view::npos) {
    end = cookies.find(';', begin);
    auto cookie = cookies.substr(begin, end - begin);
    auto pos = cookie.find('=');
    if (pos != std::string_view::npos) {
      cookies_[cookie.substr(0, pos)] = cookie.substr(pos + 1);
    }
    begin = end + 1;
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
  [[likely]] case Rule::Disruptive::BLOCK: {
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
      // If used with parameter "phase", allow will cause the engine to stop processing the current
      // phase. Other phases will continue as normal.
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

  return std::nullopt;
}
} // namespace Wge