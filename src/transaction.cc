#include "transaction.h"

#include <chrono>
#include <format>

#include "common/empty_string.h"
#include "common/log.h"
#include "common/try.h"
#include "engine.h"

namespace SrSecurity {
const Transaction::RandomInitHelper Transaction::random_init_helper_;

Transaction::Transaction(const Engine& engin) : engin_(engin) {
  initUniqueId();
  tx_.reserve(100);
}

void Transaction::processConnection(ConnectionExtractor conn_extractor) {
  SRSECURITY_LOG_TRACE("====process connection====");
  extractor_.connection_extractor_ = std::move(conn_extractor);
}

void Transaction::processUri(UriExtractor uri_extractor) {
  SRSECURITY_LOG_TRACE("====process uri====");
  extractor_.uri_extractor_ = std::move(uri_extractor);
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

void Transaction::createVariable(std::string&& name, Common::Variant&& value) {
  auto iter = tx_.find(name);
  if (iter == tx_.end()) {
    tx_.emplace(std::move(name), std::move(value));
  } else {
    iter->second = std::move(value);
  }
}

void Transaction::removeVariable(const std::string& name) { tx_.erase(name); }

void Transaction::increaseVariable(const std::string& name, int value) {
  auto iter = tx_.find(name);
  if (iter != tx_.end()) {
    iter->second = std::get<int>(iter->second) + value;
  }
}

const Common::Variant& Transaction::getVariable(const std::string& name) const {
  auto iter = tx_.find(name);
  if (iter != tx_.end()) {
    return iter->second;
  }

  return EMPTY_VARIANT;
}

void Transaction::setVariable(const std::string& name, std::string&& value) {
  auto iter = tx_.find(name);
  if (iter != tx_.end()) {
    iter->second = std::move(value);
  }
}

void Transaction::setVariable(const std::string& name, int value) {
  auto iter = tx_.find(name);
  if (iter != tx_.end()) {
    iter->second = value;
  }
}

bool Transaction::hasVariable(const std::string& name) const { return tx_.find(name) != tx_.end(); }

void Transaction::setMatched(size_t index, const std::string_view& value) {
  assert(index < matched_.size());
  if (index < matched_.size()) {
    matched_[index] = value;
  }
}

const std::string_view* Transaction::getMatched(size_t index) const {
  assert(index < matched_.size());
  if (index < matched_.size()) {
    return &matched_[index];
  }

  return nullptr;
}

void Transaction::initUniqueId() {
  using namespace std::chrono;
  uint64_t timestamp =
      time_point_cast<std::chrono::milliseconds>(system_clock::now()).time_since_epoch().count();
  int random = ::rand() % 100000 + 100000;
  unique_id_ = std::format("{}.{}", timestamp, random);
}

inline void Transaction::process(int phase) {
  // Get the rules in the given phase
  auto& rules = engin_.rules(phase);

  // Traverse the rules and evaluate them
  for (auto iter = rules.begin(); iter != rules.end();) {
    // Evaluate the rule
    auto& rule = *iter;
    auto is_matched = rule->evaluate(*this, extractor_);

    if (!is_matched) {
      ++iter;
      continue;
    }

    // Log the matched rule
    if (log_callback_) [[likely]] {
      const SrSecurity::Rule* default_action = engin_.defaultActions(rule->phase());
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
      auto next_rule_iter = engin_.marker(skip_after, rule->phase());
      if (next_rule_iter.has_value()) [[likely]] {
        iter = next_rule_iter.value();
        continue;
      }
    }

    // If skip and skipAfter are not set, then continue to the next rule
    ++iter;
  }
}

} // namespace SrSecurity