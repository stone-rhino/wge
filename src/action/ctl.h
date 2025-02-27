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