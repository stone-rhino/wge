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
#include "ctl.h"

#include <stdlib.h>

#include "../common/assert.h"
#include "../engine.h"

namespace Wge {
namespace Action {
Ctl::Ctl(CtlType type, std::any&& value) : type_(type), value_(std::move(value)) {
  switch (type_) {
  case CtlType::AuditEngine:
    evaluate_func_ = std::bind(&Ctl::evaluate_audit_engine, this, std::placeholders::_1);
    break;
  case CtlType::AuditLogParts:
    evaluate_func_ = std::bind(&Ctl::evaluate_audit_log_parts, this, std::placeholders::_1);
    break;
  case CtlType::RequestBodyAccess:
    evaluate_func_ = std::bind(&Ctl::evaluate_request_body_access, this, std::placeholders::_1);
    break;
  case CtlType::RequestBodyProcessor:
    evaluate_func_ = std::bind(&Ctl::evaluate_request_body_processor, this, std::placeholders::_1);
    break;
  case CtlType::RuleEngine:
    evaluate_func_ = std::bind(&Ctl::evaluate_rule_engine, this, std::placeholders::_1);
    break;
  case CtlType::RuleRemoveById:
    evaluate_func_ = std::bind(&Ctl::evaluate_rule_remove_by_id, this, std::placeholders::_1);
    break;
  case CtlType::RuleRemoveByIdRange:
    evaluate_func_ = std::bind(&Ctl::evaluate_rule_remove_by_id_range, this, std::placeholders::_1);
    break;
  case CtlType::RuleRemoveByTag:
    evaluate_func_ = std::bind(&Ctl::evaluate_rule_remove_by_tag, this, std::placeholders::_1);
    break;
  case CtlType::RuleRemoveTargetById:
    evaluate_func_ =
        std::bind(&Ctl::evaluate_rule_remove_target_by_id, this, std::placeholders::_1);
    break;
  case CtlType::RuleRemoveTargetByTag:
    evaluate_func_ =
        std::bind(&Ctl::evaluate_rule_remove_target_by_tag, this, std::placeholders::_1);
    break;
  default:
    UNREACHABLE();
    break;
  }
}

void Ctl::initRules(const Engine& engin) {
  switch (type_) {
  case CtlType::RuleRemoveById: {
    uint64_t id = std::any_cast<uint64_t>(value_);
    const Rule* rule = engin.findRuleById(id);
    if (rule) {
      const int phase = rule->phase();
      assert(phase >= 1 && phase <= PHASE_TOTAL);
      if (phase >= 1 || phase <= PHASE_TOTAL) {
        rules_[phase - 1].emplace(rule);
      }
    }
  } break;
  case CtlType::RuleRemoveByIdRange: {
    const std::pair<uint64_t, uint64_t>& id_range =
        std::any_cast<std::pair<uint64_t, uint64_t>>(value_);
    for (uint64_t id = id_range.first; id <= id_range.second; ++id) {
      const Rule* rule = engin.findRuleById(id);
      if (rule) {
        const int phase = rule->phase();
        assert(phase >= 1 && phase <= PHASE_TOTAL);
        if (phase >= 1 || phase <= PHASE_TOTAL) {
          rules_[phase - 1].emplace(rule);
        }
      }
    }
  } break;
  case CtlType::RuleRemoveByTag: {
    const std::string& tag = std::any_cast<std::string>(value_);
    engin.findRuleByTag(tag, rules_);
  } break;
  case CtlType::RuleRemoveTargetById: {
    const auto& id_and_variables =
        std::any_cast<std::pair<uint64_t, std::vector<std::shared_ptr<Variable::VariableBase>>>>(
            value_);
    const Rule* rule = engin.findRuleById(id_and_variables.first);
    if (rule) {
      const int phase = rule->phase();
      assert(phase >= 1 && phase <= PHASE_TOTAL);
      if (phase >= 1 || phase <= PHASE_TOTAL) {
        rules_[phase - 1].emplace(rule);
      }
    }
  } break;
  case CtlType::RuleRemoveTargetByTag: {
    const auto& tag_and_variables =
        std::any_cast<std::pair<std::string, std::vector<std::shared_ptr<Variable::VariableBase>>>>(
            value_);
    engin.findRuleByTag(tag_and_variables.first, rules_);
  } break;
  default:
    // The other types don't need to initialize the rules
    break;
  }
}

void Ctl::evaluate_audit_engine(Transaction& t) const {
  AuditLogConfig::AuditEngine option = std::any_cast<AuditLogConfig::AuditEngine>(value_);
  UNREACHABLE();
  throw "Not implemented!";
}

void Ctl::evaluate_audit_log_parts(Transaction& t) const {
  const std::string& parts = std::any_cast<std::string>(value_);
  UNREACHABLE();
  throw "Not implemented!";
}

void Ctl::evaluate_request_body_access(Transaction& t) const {
  EngineConfig::Option option = std::any_cast<EngineConfig::Option>(value_);
  UNREACHABLE();
  throw "Not implemented!";
}

void Ctl::evaluate_request_body_processor(Transaction& t) const {
  BodyProcessorType type = std::any_cast<BodyProcessorType>(value_);
  t.setRequestBodyProcessor(type);
}

void Ctl::evaluate_rule_engine(Transaction& t) const {
  EngineConfig::Option option = std::any_cast<EngineConfig::Option>(value_);
  UNREACHABLE();
  throw "Not implemented!";
}

void Ctl::evaluate_rule_remove_by_id(Transaction& t) const { t.removeRule(rules_); }

void Ctl::evaluate_rule_remove_by_id_range(Transaction& t) const {
  const std::pair<uint64_t, uint64_t>& id_range =
      std::any_cast<std::pair<uint64_t, uint64_t>>(value_);
  t.removeRule(rules_);
}

void Ctl::evaluate_rule_remove_by_tag(Transaction& t) const {
  const std::string& tag = std::any_cast<std::string>(value_);
  t.removeRule(rules_);
}

void Ctl::evaluate_rule_remove_target_by_id(Transaction& t) const {
  const auto& id_and_variables =
      std::any_cast<std::pair<uint64_t, std::vector<std::shared_ptr<Variable::VariableBase>>>>(
          value_);
  t.removeRuleTarget(rules_, id_and_variables.second);
}

void Ctl::evaluate_rule_remove_target_by_tag(Transaction& t) const {
  const auto& tag_and_variables =
      std::any_cast<std::pair<std::string, std::vector<std::shared_ptr<Variable::VariableBase>>>>(
          value_);
  t.removeRuleTarget(rules_, tag_and_variables.second);
}

} // namespace Action
} // namespace Wge