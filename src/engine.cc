#include "engine.h"

#include "antlr4/parser.h"
#include "common/assert.h"
#include "common/log.h"

std::thread::id main_thread_id;

namespace SrSecurity {
Engine::Engine() : parser_(std::make_shared<Antlr4::Parser>()) {
  // We assume that it can only be constructed in the main thread
  main_thread_id = std::this_thread::get_id();
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
}

static std::vector<const Rule*> empty_rules;
const std::vector<const Rule*>& Engine::defaultActions(int phase) const {
  assert(phase >= 1 && phase <= 5);
  if (phase >= 1 && phase <= 5) {
    return default_actions_[phase - 1];
  } else {
    return empty_rules;
  }
}

const std::vector<const Rule*>& Engine::rules(int phase) const {
  assert(phase >= 1 && phase <= 5);
  if (phase >= 1 && phase <= 5) {
    return rules_[phase - 1];
  } else {
    return empty_rules;
  }
}

TransactionPtr Engine::makeTransaction() const {
  return std::unique_ptr<Transaction>(new Transaction(*this));
}

std::optional<const std::vector<const Rule*>::iterator> Engine::marker(const std::string& name,
                                                                       int phase) const {
  assert(phase >= 1 && phase <= phase_total_);
  if (phase < 1 || phase > phase_total_) {
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
    assert(phase >= 1 && phase <= phase_total_);
    if (phase < 1 || phase > phase_total_) {
      SRSECURITY_LOG(warn, "phase {} invalid.", phase);
      continue;
    }
    default_actions_.at(phase - 1).emplace_back(rule.get());
  }
}

void Engine::initRules() {
  auto& rules = parser_->rules();

  // Sets the rules for each phase
  for (auto& rule : rules) {
    auto phase = rule->phase();
    assert(phase >= 1 && phase <= phase_total_);
    if (phase < 1 || phase > phase_total_) {
      SRSECURITY_LOG(warn, "phase {} invalid. rule id:{}", phase, rule->id());
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
    for (int i = 0; i < phase_total_; ++i) {
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

} // namespace SrSecurity