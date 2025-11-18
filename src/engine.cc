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
    : parser_(std::make_unique<Antlr4::Parser>()) {
  // We assume that it can only be constructed in the main thread
  main_thread_id = std::this_thread::get_id();

  // Initialize the log
  Common::Log::init(level, log_file);

  // Initialize the random seed
  ::srand(::time(nullptr));
}

Engine::~Engine() = default;

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

  initRules();

  is_init_ = true;
}

const Rule* Engine::defaultActions(RulePhaseType phase) const {
  assert(phase >= 1 && phase <= PHASE_TOTAL);
  auto& default_action_rule = parser_->defaultActions()[phase - 1];
  return default_action_rule.has_value() ? &default_action_rule.value() : nullptr;
}

const std::vector<Rule>& Engine::rules(RulePhaseType phase) const {
  assert(phase >= 1 && phase <= PHASE_TOTAL);
  return parser_->rules()[phase - 1];
}

TransactionPtr Engine::makeTransaction() const {
  assert(is_init_);
  return std::unique_ptr<Transaction>(new Transaction(*this, parser_->getTxVariableIndexSize()));
}

const EngineConfig& Engine::config() const { return parser_->engineConfig(); }

const AuditLogConfig& Engine::auditLogConfig() const { return parser_->auditLogConfig(); }

ParseXmlIntoArgsOption Engine::parseXmlIntoArgsOption() const {
  return parser_->parseXmlIntoArgsOption();
}

const Rule* Engine::findRuleById(uint64_t id) const {
  // An efficient and rational design should not call this method in the worker thread.
  // This assert check that this method can only be called in the main thread
  ASSERT_IS_MAIN_THREAD();

  return parser_->findRuleById(id);
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

  auto rules = parser_->findRuleByTag(tag);
  for (auto rule : rules) {
    auto phase = rule->phase();
    assert(phase >= 1 && phase <= PHASE_TOTAL);
    if (phase >= 1 || phase <= PHASE_TOTAL) {
      rule_set[phase - 1].emplace(rule);
    }
  }
}

std::optional<size_t> Engine::getTxVariableIndex(const std::string& name) const {
  return parser_->getTxVariableIndex(name, false);
}

std::string_view Engine::getTxVariableIndexReverse(size_t index) const {
  return parser_->getTxVariableIndexReverse(index);
}

void Engine::initRules() {
  auto& markers = parser_->markers();
  for (RulePhaseType phase = 1; phase <= PHASE_TOTAL; ++phase) {
    auto& rules = parser_->rules()[phase - 1];

    // Initialize the except variables
    for (auto& rule : rules) {
      rule.initExceptVariables();
    }

    // Initialize the Pmf operator
    for (auto& rule : rules) {
      rule.initPmfOperator(parser_->engineConfig().pmf_serialize_dir_);
    }

    // Initialize the flags according to the default action rule
    for (auto& rule : rules) {
      auto default_action_rule = defaultActions(phase);
      if (default_action_rule) {
        rule.initFlags(*default_action_rule);
      }
    }

    // Initialize the rules ctl
    for (auto& rule : rules) {
      auto& actions = rule.actions();
      for (auto& action : actions) {
        if (::strncmp("ctl", action->name(), 3) == 0) {
          auto ctl = dynamic_cast<Action::Ctl*>(action.get());
          ctl->initRules(*this);
        }
      }
    }

    // Initialize the skip of rules
    for (auto& rule : rules) {
      std::string_view skip_after = rule.skipAfter();
      if (!skip_after.empty() && rule.skip() == 0) {
        auto marker_iter = markers.find(skip_after);
        if (marker_iter != markers.end()) {
          auto pre_rule_index = marker_iter->second[phase - 1];
          if (pre_rule_index != -1 && pre_rule_index > rule.index()) {
            // Transform the skip_after to skip
            size_t skip = pre_rule_index - rule.index();
            rule.skip(skip);
          }
        }
      }
    }
  }
}

} // namespace Wge