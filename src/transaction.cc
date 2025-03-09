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
  initUniqueId();
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

void Transaction::processUri(std::string_view uri) {
  SRSECURITY_LOG_TRACE("====process uri====");

  // parse the uri
  uri_ = uri;
  auto pos = uri.find(' ');
  if (pos != std::string_view::npos) {
    // parse the method
    uri_info_.method_ = uri.substr(0, pos);

    // parse the path
    uri.remove_prefix(pos + 1);
    pos = uri.find(' ');
    if (pos != std::string_view::npos) {
      uri_info_.path_ = uri.substr(0, pos);
    }

    // parse the query
    auto pos_question = uri_info_.path_.find('?');
    if (pos_question != std::string_view::npos) {
      uri_info_.query_ = uri_info_.path_.substr(pos_question + 1);
      uri_info_.path_.remove_suffix(uri_info_.path_.size() - pos_question);
    }

    // parse the protocol and verison
    uri.remove_prefix(pos + 1);
    pos = uri.find('/');
    if (pos != std::string_view::npos) {
      uri_info_.protocol_ = uri.substr(0, pos);
      uri.remove_prefix(pos + 1);
      uri_info_.version_ = uri;
    }

    SRSECURITY_LOG_TRACE("method: {}, path: {}, query: {}, protocol: {}, version: {}",
                         uri_info_.method_, uri_info_.path_, uri_info_.query_, uri_info_.protocol_,
                         uri_info_.version_);
  }
}

void Transaction::processRequestHeaders(HeaderExtractor header_extractor,
                                        std::function<void(const Rule&)> log_callback) {
  SRSECURITY_LOG_TRACE("====process request headers====");
  extractor_.request_header_extractor_ = std::move(header_extractor);
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

void Transaction::processResponseHeaders(HeaderExtractor header_extractor,
                                         std::function<void(const Rule&)> log_callback) {
  SRSECURITY_LOG_TRACE("====process response headers====");
  extractor_.response_header_extractor_ = std::move(header_extractor);
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

void Transaction::setMatched(size_t index, std::string_view value) {
  assert(index < matched_.size());
  if (index < matched_.size()) {
    matched_[index] = value;
  }
}

const Common::Variant& Transaction::getMatched(size_t index) const {
  assert(index < matched_.size());
  if (index < matched_.size()) {
    return matched_[index];
  }

  return EMPTY_VARIANT;
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
} // namespace SrSecurity