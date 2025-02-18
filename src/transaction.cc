#include "transaction.h"

#include <charconv>
#include <chrono>
#include <format>

#include <assert.h>

#include "common/empty_string.h"
#include "common/try.h"
#include "engine.h"

namespace SrSecurity {
const Transaction::RandomInitHelper Transaction::random_init_helper_;

Transaction::Transaction(const Engine& engin) : engin_(engin) { initUniqueId(); }

void Transaction::processConnection(ConnectionExtractor conn_extractor) {
  extractor_.connection_extractor_ = std::move(conn_extractor);
}

void Transaction::processUri(UriExtractor uri_extractor) {
  extractor_.uri_extractor_ = std::move(uri_extractor);
}

void Transaction::processRequestHeaders(HeaderExtractor header_extractor) {
  extractor_.request_header_extractor_ = std::move(header_extractor);

  // Get the rules of phase 1
  auto& rules = engin_.rules(1);

  // Traverse the rules and evaluate them
  for (auto iter = rules.begin(); iter != rules.end();) {
    // Evaluate the rule
    auto& rule = *iter;
    auto is_matched = rule->evaluate(*this, extractor_);

    // Skip the rules if current rule that has a skip action or skipAfter action is matched
    if (is_matched) {
      // Process the skip action
      int skip = rule->skip();
      if (skip > 0) [[unlikely]] {
        iter += skip;
        continue;
      }

      // Process the skipAfter action
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
    } else {
      ++iter;
    }
  }
}

void Transaction::processRequestBody(BodyExtractor body_extractor) {
  extractor_.reqeust_body_extractor_ = std::move(body_extractor);
}

void Transaction::processResponseHeaders(HeaderExtractor header_extractor) {
  extractor_.response_header_extractor_ = std::move(header_extractor);
}

void Transaction::processResponseBody(BodyExtractor body_extractor) {
  extractor_.response_body_extractor_ = std::move(body_extractor);
}

void Transaction::createVariable(std::string&& name, int value) {
  auto iter = tx_.find(name);
  if (iter == tx_.end()) {
    tx_.emplace(std::move(name), std::to_string(value));
  } else {
    iter->second = std::to_string(value);
  }
}

void Transaction::createVariable(std::string&& name, std::string&& value) {
  auto iter = tx_.find(name);
  if (iter == tx_.end()) {
    tx_.emplace(std::move(name), value);
  }
}

void Transaction::removeVariable(const std::string& name) { tx_.erase(name); }

void Transaction::increaseVariable(const std::string& name, int value) {
  auto iter = tx_.find(name);
  if (iter != tx_.end()) {
    int v = ::atol(iter->second.c_str());
    v += value;
    iter->second = std::to_string(v);
  }
}

const std::string& Transaction::getVariable(const std::string& name) const {
  auto iter = tx_.find(name);
  if (iter != tx_.end()) {
    return iter->second;
  }

  return EMPTY_STRING;
}

int Transaction::getVariableInt(const std::string& name) const {
  std::string_view str_value = getVariable(name);
  int int_value = 0;
  std::from_chars(str_value.data(), str_value.data() + str_value.size(), int_value);
  return int_value;
}

void Transaction::setVariable(const std::string& name, std::string&& value) {
  auto iter = tx_.find(name);
  if (iter != tx_.end()) {
    iter->second = std::move(value);
  }
}

void Transaction::setVariableInt(const std::string& name, int value) {
  auto iter = tx_.find(name);
  if (iter != tx_.end()) {
    iter->second = std::to_string(value);
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

} // namespace SrSecurity