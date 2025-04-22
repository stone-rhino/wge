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
#pragma once

#include <any>
#include <array>
#include <functional>
#include <map>
#include <string>
#include <unordered_set>

#include "action_base.h"

#include "../macro/macro_base.h"
#include "../variable/variable_base.h"

namespace SrSecurity {
namespace Action {
/**
 * Changes ModSecurity configuration on transient, per-transaction basis. Any changes made using
 * this action will affect only the transaction in which the action is executed. The default
 * configuration, as well as the other transactions running in parallel, will be unaffected.
 */
class Ctl : public ActionBase {
  DECLARE_ACTION_NAME(ctl);

public:
  enum class CtlType {
    AuditEngine = 0,
    AuditLogParts,
    RequestBodyAccess,
    RequestBodyProcessor,
    RuleEngine,
    RuleRemoveById,
    RuleRemoveByIdRange,
    RuleRemoveByTag,
    RuleRemoveTargetById,
    RuleRemoveTargetByTag
  };

public:
  Ctl(CtlType type, std::any&& value);

public:
  void evaluate(Transaction& t) const override { evaluate_func_(t); }

public:
  /**
   * Initialize the rules that will be removed or updated.
   *
   * Because the rules that will be removed or updated are stored in the Ctl instance, but
   * the rules are may be not created yet when the Ctl instance is created. So, we need to
   * initialize the rules before evaluating the Ctl instance and only initialize once in the life of
   * the Ctl instance.
   * Why don't we initialize the rules in the evaluate method? Because performance is important. We
   * don't want to find the rules every time the Ctl instance is evaluated.
   * @param engin the engine instance
   */
  void initRules(const Engine& engin);

private:
  void evaluate_audit_engine(Transaction& t) const;
  void evaluate_audit_log_parts(Transaction& t) const;
  void evaluate_request_body_access(Transaction& t) const;
  void evaluate_request_body_processor(Transaction& t) const;
  void evaluate_rule_engine(Transaction& t) const;
  void evaluate_rule_remove_by_id(Transaction& t) const;
  void evaluate_rule_remove_by_id_range(Transaction& t) const;
  void evaluate_rule_remove_by_tag(Transaction& t) const;
  void evaluate_rule_remove_target_by_id(Transaction& t) const;
  void evaluate_rule_remove_target_by_tag(Transaction& t) const;

private:
  CtlType type_;
  std::any value_;
  std::function<void(Transaction&)> evaluate_func_;
  std::array<std::unordered_set<const Rule*>, PHASE_TOTAL> rules_;
};
} // namespace Action
} // namespace SrSecurity