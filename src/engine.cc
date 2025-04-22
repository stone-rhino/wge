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
#include "engine.h"

#include "action/ctl.h"
#include "antlr4/parser.h"
#include "common/assert.h"
#include "common/log.h"

std::thread::id main_thread_id;

namespace Wge {
Engine::Engine(spdlog::level::level_enum level, const std::string& log_file)
    : parser_(std::make_shared<Antlr4::Parser>()) {
  // We assume that it can only be constructed in the main thread
  main_thread_id = std::this_thread::get_id();

  // Initialize the log
  Common::Log::init(level, log_file);
}

std::expected<bool, std::string> Engine::loadFromFile(const std::string& file_path) {
  // An efficient and rational design should not call this method in the worker thread.
  // This assert check that this method can only be called in the main thread
  ASSERT_IS_MAIN_THREAD();

  return parser_->loadFromFile(file_path);
}

std::expected<bool, std::string> Engine::load(const std::string& directive) {
  // An efficient and rational design should not call this method in the worker thread.
  // This assert check that this method can only be called in the main thread
  ASSERT_IS_MAIN_THREAD();

  return parser_->load(directive);
}

void Engine::init() {
  // An efficient and rational design should not call this method in the worker thread.
  // This assert check that this method can only be called in the main thread
  ASSERT_IS_MAIN_THREAD();

  initDefaultActions();
  initRules();
  initMakers();

  is_init_ = true;
}

const Rule* Engine::defaultActions(int phase) const {
  assert(phase >= 1 && phase <= PHASE_TOTAL);
  return default_actions_[phase - 1];
}

const std::vector<const Rule*>& Engine::rules(int phase) const {
  assert(phase >= 1 && phase <= PHASE_TOTAL);
  return rules_[phase - 1];
}

TransactionPtr Engine::makeTransaction() const {
  assert(is_init_);
  return std::unique_ptr<Transaction>(new Transaction(*this, parser_->getTxVariableIndexSize()));
}

const EngineConfig& Engine::config() const { return parser_->engineConfig(); }

const AuditLogConfig& Engine::auditLogConfig() const { return parser_->auditLogConfig(); }

const Rule* Engine::findRuleById(uint64_t id) const {
  // An efficient and rational design should not call this method in the worker thread.
  // This assert check that this method can only be called in the main thread
  ASSERT_IS_MAIN_THREAD();

  auto iter = parser_->findRuleById(id);
  if (iter != parser_->rules().end()) {
    return iter->get();
  } else {
    return nullptr;
  }
}

void Engine::findRuleByTag(
    const std::string& tag,
    std::array<std::unordered_set<const Rule*>, PHASE_TOTAL>& rule_set) const {
  // An efficient and rational design should not call this method in the worker thread.
  // This assert check that this method can only be called in the main thread
  ASSERT_IS_MAIN_THREAD();

  // Clear the rule set
  for (auto& rules : rule_set) {
    rules.clear();
  }

  auto range = parser_->findRuleByTag(tag);
  for (auto iter = range.first; iter != range.second; ++iter) {
    const int phase = (*iter->second)->phase();
    assert(phase >= 1 && phase <= PHASE_TOTAL);
    if (phase >= 1 || phase <= PHASE_TOTAL) {
      rule_set[phase - 1].emplace((*iter->second).get());
    }
  }
}

std::optional<size_t> Engine::getTxVariableIndex(const std::string& name) const {
  return parser_->getTxVariableIndex(name, false);
}

std::string_view Engine::getTxVariableIndexReverse(size_t index) const {
  return parser_->getTxVariableIndexReverse(index);
}

std::optional<const std::vector<const Rule*>::iterator> Engine::marker(const std::string& name,
                                                                       int phase) const {
  assert(phase >= 1 && phase <= PHASE_TOTAL);
  if (phase < 1 || phase > PHASE_TOTAL) {
    return {};
  }

  auto iter = markers_.find(name);
  if (iter != markers_.end()) {
    if (iter->second.prevRuleIter(phase).has_value()) {
      return std::next(iter->second.prevRuleIter(phase).value());
    } else {
      return {};
    }
  }

  return {};
}

void Engine::initDefaultActions() {
  auto& rules = parser_->defaultActions();

  // Sets the default actions for each phase
  for (auto& rule : rules) {
    auto phase = rule->phase();
    assert(phase >= 1 && phase <= PHASE_TOTAL);
    if (phase < 1 || phase > PHASE_TOTAL) {
      WGE_LOG_WARN("phase {} invalid.", phase);
      continue;
    }
    default_actions_[phase - 1] = rule.get();
  }
}

void Engine::initRules() {
  auto& rules = parser_->rules();

  // Initialize the except variables
  for (auto& rule : rules) {
    rule->initExceptVariables();
  }

  // Initialize the rules ctl
  for (auto& rule : rules) {
    auto& actions = rule->actions();
    for (auto& action : actions) {
      if (::strncmp("ctl", action->name(), 3) == 0) {
        auto ctl = dynamic_cast<Action::Ctl*>(action.get());
        ctl->initRules(*this);
      }
    }
  }

  // Sets the rules for each phase
  for (auto& rule : rules) {
    auto phase = rule->phase();
    assert(phase >= 1 && phase <= PHASE_TOTAL);
    if (phase < 1 || phase > PHASE_TOTAL) {
      WGE_LOG_WARN("phase {} invalid. rule id:{}", phase, rule->id());
      continue;
    }
    rules_.at(phase - 1).emplace_back(rule.get());
  }
}

void Engine::initMakers() {
  auto& markers = const_cast<std::list<Marker>&>(parser_->markers());

  // Traverse the markers and find the rule that the marker points to
  for (auto& marker : markers) {
    // Traverse the rules in each phase
    for (int i = 0; i < PHASE_TOTAL; ++i) {
      // Find the rule that the marker points to
      for (auto iter = rules_[i].begin(); iter != rules_[i].end(); ++iter) {
        if (*iter == marker.prevRule(i + 1)) {
          marker.prevRuleIter(iter, i + 1);
          break;
        }
      }
    }
    markers_.emplace(marker.name(), marker);
  }
}

} // namespace Wge