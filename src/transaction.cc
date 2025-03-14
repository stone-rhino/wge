#include "transaction.h"

#include <chrono>
#include <format>

#include "action/set_var.h"
#include "common/assert.h"
#include "common/empty_string.h"
#include "common/log.h"
#include "common/try.h"
#include "engine.h"

namespace SrSecurity {
const Transaction::RandomInitHelper Transaction::random_init_helper_;

// To avoid the dynamic memory allocation, we allocate the memory for the variable vector in
// advance. We assume that the count of variable that the key of varabile contains macro is less
// than variable_key_with_macro_size.
constexpr size_t variable_key_with_macro_size = 100;

Transaction::Transaction(const Engine& engin, size_t literal_key_size)
    : engine_(engin), tx_variables_(literal_key_size + variable_key_with_macro_size),
      tx_variables_buffer_(literal_key_size + variable_key_with_macro_size),
      literal_key_size_(literal_key_size) {
  tx_variables_.resize(literal_key_size);
  tx_variables_buffer_.resize(literal_key_size);
  assert(tx_variables_.capacity() == literal_key_size + variable_key_with_macro_size);
  assert(tx_variables_buffer_.capacity() == literal_key_size + variable_key_with_macro_size);
}

void Transaction::processConnection(std::string_view downstream_ip, short downstream_port,
                                    std::string_view upstream_ip, short upstream_port) {
  SRSECURITY_LOG_TRACE("====process connection====");
  connection_info_.downstream_ip_ = downstream_ip;
  connection_info_.downstream_port_ = downstream_port;
  connection_info_.upstream_ip_ = upstream_ip;
  connection_info_.upstream_port_ = upstream_port;
}

void Transaction::processUri(std::string_view request_line) {
  SRSECURITY_LOG_TRACE("====process uri====");

  // Parse the request line
  request_line_ = request_line;
  auto pos = request_line.find(' ');
  if (pos != std::string_view::npos) {
    // Parse the method
    requset_line_info_.method_ = request_line.substr(0, pos);

    // Parse the uri
    request_line.remove_prefix(pos + 1);
    pos = request_line.find(' ');
    if (pos != std::string_view::npos) {
      requset_line_info_.uri_ = request_line.substr(0, pos);
      requset_line_info_.uri_raw_ = requset_line_info_.uri_;
    }

    // Parse the query
    auto pos_question = requset_line_info_.uri_.find('?');
    if (pos_question != std::string_view::npos) {
      requset_line_info_.query_ = requset_line_info_.uri_.substr(pos_question + 1);
      requset_line_info_.uri_.remove_suffix(requset_line_info_.uri_.size() - pos_question);
    }

    // Parse the relative uri
    requset_line_info_.relative_uri_ = requset_line_info_.uri_;
    if (requset_line_info_.relative_uri_.starts_with("http://")) {
      auto pos = requset_line_info_.relative_uri_.find('/', 7);
      if (pos != std::string_view::npos) {
        requset_line_info_.relative_uri_.remove_prefix(pos);
      }
    } else if (requset_line_info_.relative_uri_.starts_with("https://")) {
      auto pos = requset_line_info_.relative_uri_.find('/', 8);
      if (pos != std::string_view::npos) {
        requset_line_info_.relative_uri_.remove_prefix(pos);
      }
    }

    // Parse the protocol and verison
    request_line.remove_prefix(pos + 1);
    pos = request_line.find('/');
    if (pos != std::string_view::npos) {
      requset_line_info_.protocol_ = request_line;
      request_line.remove_prefix(pos + 1);
      requset_line_info_.version_ = request_line;
    }

    // Init the query params
    initQueryParams();

    SRSECURITY_LOG_TRACE("method: {}, uri: {}, query: {}, protocol: {}, version: {}",
                         requset_line_info_.method_, requset_line_info_.uri_,
                         requset_line_info_.query_, requset_line_info_.protocol_,
                         requset_line_info_.version_);
  }
}

void Transaction::processUri(std::string_view uri, std::string_view method,
                             std::string_view version) {
  SRSECURITY_LOG_TRACE("====process uri====");

  // method
  requset_line_info_.method_ = method;

  // uri
  requset_line_info_.uri_raw_ = uri;
  requset_line_info_.uri_ = uri;

  // Parse the query
  auto pos_question = requset_line_info_.uri_.find('?');
  if (pos_question != std::string_view::npos) {
    requset_line_info_.query_ = requset_line_info_.uri_.substr(pos_question + 1);
    requset_line_info_.uri_.remove_suffix(requset_line_info_.uri_.size() - pos_question);
  }

  // Parse the relative uri
  requset_line_info_.relative_uri_ = requset_line_info_.uri_;
  if (requset_line_info_.relative_uri_.starts_with("http://")) {
    auto pos = requset_line_info_.relative_uri_.find('/', 7);
    if (pos != std::string_view::npos) {
      requset_line_info_.relative_uri_.remove_prefix(pos);
    }
  } else if (requset_line_info_.relative_uri_.starts_with("https://")) {
    auto pos = requset_line_info_.relative_uri_.find('/', 8);
    if (pos != std::string_view::npos) {
      requset_line_info_.relative_uri_.remove_prefix(pos);
    }
  }

  // protocol and verison
  requset_line_info_.protocol_ = "HTTP";
  requset_line_info_.version_ = version;

  // Combine the request line
  request_line_buffer_.reserve(
      requset_line_info_.method_.size() + requset_line_info_.uri_raw_.size() +
      requset_line_info_.protocol_.size() + requset_line_info_.version_.size() + 3);
  request_line_buffer_ += requset_line_info_.method_;
  request_line_buffer_ += ' ';
  request_line_buffer_ += requset_line_info_.uri_raw_;
  request_line_buffer_ += ' ';
  request_line_buffer_ += requset_line_info_.protocol_;
  request_line_buffer_ += '/';
  request_line_buffer_ += requset_line_info_.version_;
  request_line_ = request_line_buffer_;

  // Init the query params
  initQueryParams();

  SRSECURITY_LOG_TRACE("method: {}, uri: {}, query: {}, protocol: {}, version: {}",
                       requset_line_info_.method_, requset_line_info_.uri_,
                       requset_line_info_.query_, requset_line_info_.protocol_,
                       requset_line_info_.version_);
}

void Transaction::processRequestHeaders(HeaderFind request_header_find,
                                        HeaderTraversal request_header_traversal,
                                        size_t header_count,
                                        std::function<void(const Rule&)> log_callback) {
  SRSECURITY_LOG_TRACE("====process request headers====");
  extractor_.request_header_find_ = std::move(request_header_find);
  extractor_.request_header_traversal_ = std::move(request_header_traversal);
  extractor_.request_header_count_ = header_count;
  log_callback_ = std::move(log_callback);
  process(1);
}

void Transaction::processRequestBody(BodyExtractor body_extractor,
                                     std::function<void(const Rule&)> log_callback) {
  SRSECURITY_LOG_TRACE("====process request body====");
  extractor_.reqeust_body_extractor_ = std::move(body_extractor);
  log_callback_ = std::move(log_callback);
  process(2);
}

void Transaction::processResponseHeaders(HeaderFind response_header_find,
                                         HeaderTraversal response_header_traversal,
                                         size_t response_header_count,
                                         std::function<void(const Rule&)> log_callback) {
  SRSECURITY_LOG_TRACE("====process response headers====");
  extractor_.response_header_find_ = std::move(response_header_find);
  extractor_.response_header_traversal_ = std::move(response_header_traversal);
  extractor_.response_header_count_ = response_header_count;
  log_callback_ = std::move(log_callback);
  process(3);
}

void Transaction::processResponseBody(BodyExtractor body_extractor,
                                      std::function<void(const Rule&)> log_callback) {
  SRSECURITY_LOG_TRACE("====process response body====");
  extractor_.response_body_extractor_ = std::move(body_extractor);
  log_callback_ = std::move(log_callback);
  process(4);
}

void Transaction::setVariable(size_t index, const Common::Variant& value) {
  assert(index < tx_variables_.size());
  if (index < tx_variables_.size()) {
    if (IS_INT_VARIANT(value)) [[likely]] {
      tx_variables_[index] = std::get<int>(value);
    } else if (IS_STRING_VIEW_VARIANT(value)) {
      // The tx_variables_ store the value as a variant(std::string_view), and it will be invalid if
      // it's reference is invalid. So we copy the value to the tx_variables_buffer_. The
      // tx_variables_buffer_ store the value as a string, and it will be valid until the
      // transaction is destroyed. Why we don't let the Common::Variant store the value as a
      // std::string? If we do it, it seems that we don't need the tx_variables_buffer_ anymore. But
      // it will cause code diffcult to maintain. Because the Common::Variant is a variant of
      // std::monostate, int, std::string_view. If we add a new std::sting type, we must process the
      // variant in repeat places when we want to get the value as a string. So we choose to store
      // the value as a std::string_view in the Common::Variant, and copy the value to the
      // tx_variables_buffer_ when we want to store the value as a string. It's a trade-off between
      // the code maintainability and the performance.
      tx_variables_buffer_[index] = std::get<std::string_view>(value);
      tx_variables_[index] = tx_variables_buffer_[index];
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
    assert(!IS_EMPTY_VARIANT(tx_variables_[index]));
    tx_variables_[index] = EMPTY_VARIANT;
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
    auto& variant = tx_variables_[index];
    assert(IS_INT_VARIANT(variant));
    if (IS_INT_VARIANT(variant)) {
      variant = std::get<int>(variant) + value;
    }
  }
}

void Transaction::increaseVariable(const std::string& name, int value) {
  auto index = engine_.getTxVariableIndex(name);
  assert(index.has_value());
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
    return tx_variables_[index];
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

bool Transaction::hasVariable(size_t index) const {
  assert(index < tx_variables_.size());
  return index < tx_variables_.size() && !IS_EMPTY_VARIANT(tx_variables_[index]);
}

bool Transaction::hasVariable(const std::string& name) const {
  auto index = engine_.getTxVariableIndex(name);
  assert(index.has_value());
  return index.has_value() && hasVariable(index.value());
}

void Transaction::addMatched(std::string_view value) {
  if (matched_size_ < matched_.size()) [[likely]] {
    matched_[matched_size_] = value;
    ++matched_size_;
  }
}

const Common::Variant& Transaction::getMatched(size_t index) const {
  // assert(index < matched_size_);
  if (index < matched_size_) [[likely]] {
    return matched_[index];
  } else {
    SRSECURITY_LOG_WARN("The index of matched string is out of range. index: {}, matched size: {}",
                        index, matched_size_);
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

    // For performance reasons, we use a flag array that is the same size as the rules array to mark
    // the rules that need to be removed.
    auto& rule_remove_flag = rule_remove_flags_[phase - 1];
    if (rule_remove_flag.empty()) [[unlikely]] {
      rule_remove_flag.resize(engine_.rules(phase).size());
    }

    // We record the current rule index, make sure that the rules that have been evaluated will not
    // be removed. As above, it makes no sense to remove the rules that have been evaluated.
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

inline void Transaction::process(int phase) {
  if (engine_.config().is_rule_engine_ == EngineConfig::Option::Off) [[unlikely]] {
    return;
  }

  // Get the rules in the given phase
  auto& rules = engine_.rules(phase);

  // Traverse the rules and evaluate them
  auto begin = rules.begin();
  for (auto iter = begin; iter != rules.end();) {
    current_rule_index_ = std::distance(begin, iter);

    // Skip the rules that have been removed
    auto& rule_remove_flag = rule_remove_flags_[phase - 1];
    if (!rule_remove_flag.empty() && rule_remove_flag[current_rule_index_]) [[unlikely]] {
      ++iter;
      continue;
    }

    // Evaluate the rule
    auto& rule = *iter;
    auto is_matched = rule->evaluate(*this);

    // Clean the matched info
    matched_size_ = 0;
    current_variable_ = nullptr;
    current_variable_result_ = nullptr;

    if (!is_matched) [[likely]] {
      ++iter;
      continue;
    }

    // Log the matched rule
    if (log_callback_) [[likely]] {
      const SrSecurity::Rule* default_action = engine_.defaultActions(rule->phase());
      if (default_action) {
        if (rule->log().value_or(default_action->log().value_or(false))) {
          log_callback_(*rule);
        }
      } else {
        if (rule->log().value_or(false)) {
          log_callback_(*rule);
        }
      }
    }

    // Skip the rules if current rule that has a skip action or skipAfter action is matched
    int skip = rule->skip();
    if (skip > 0) [[unlikely]] {
      iter += skip;
      continue;
    }
    const std::string& skip_after = rule->skipAfter();
    if (!skip_after.empty()) [[unlikely]] {
      auto next_rule_iter = engine_.marker(skip_after, rule->phase());
      if (next_rule_iter.has_value()) [[likely]] {
        iter = next_rule_iter.value();
        continue;
      }
    }

    // If skip and skipAfter are not set, then continue to the next rule
    ++iter;
  }
}

inline std::optional<size_t> Transaction::getLocalVariableIndex(const std::string& key,
                                                                bool force_create) {
  auto iter = local_tx_variable_index_.find(key);
  if (iter == local_tx_variable_index_.end()) [[unlikely]] {
    if (force_create) [[likely]] {
      local_tx_variable_index_[key] = tx_variables_.size();
      tx_variables_.emplace_back(EMPTY_VARIANT);
      tx_variables_buffer_.emplace_back();
      return tx_variables_.size() - 1;
    } else {
      return std::nullopt;
    }
  }

  return iter->second;
}

inline void Transaction::initQueryParams() {
  // Parse the query parameters
  if (requset_line_info_.query_.empty()) [[unlikely]] {
    return;
  }

  // Parse the query parameters
  size_t begin = 0;
  size_t end = 0;
  while (end != std::string_view::npos) {
    end = requset_line_info_.query_.find('&', begin);
    auto param = requset_line_info_.query_.substr(begin, end - begin);
    auto pos = param.find('=');
    if (pos != std::string_view::npos) {
      requset_line_info_.query_params_.emplace_back(param.substr(0, pos), param.substr(pos + 1));
    } else {
      requset_line_info_.query_params_.emplace_back(param, "");
    }
    begin = end + 1;
  }
}
} // namespace SrSecurity