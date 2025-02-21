#pragma once

#include <any>

#include "action_base.h"

#include "../macro/macro_base.h"

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
    AuditEngine,
    AuditLogParts,
    RequestBodyAccess,
    RequestBodyProcessor,
    RuleEngine,
    RuleRemoveById,
    RuleRemoveByTag,
    RuleRemoveTargetById,
    RuleRemoveTargetByTag
  };

public:
  Ctl(CtlType type, std::any&& value);

public:
  void evaluate(Transaction& t) const override;

private:
  void evaluate_audit_engine(Transaction& t) const;
  void evaluate_audit_log_parts(Transaction& t) const;
  void evaluate_request_body_access(Transaction& t) const;
  void evaluate_request_body_processor(Transaction& t) const;
  void evaluate_rule_engine(Transaction& t) const;
  void evaluate_rule_remove_by_id(Transaction& t) const;
  void evaluate_rule_remove_by_tag(Transaction& t) const;
  void evaluate_rule_remove_target_by_id(Transaction& t) const;
  void evaluate_rule_remove_target_by_tag(Transaction& t) const;

private:
  CtlType type_;
  std::any value_;
};
} // namespace Action
} // namespace SrSecurity