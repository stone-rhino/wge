#include "ctl.h"

#include <stdlib.h>

#include "../common/assert.h"

namespace SrSecurity {
namespace Action {
Ctl::Ctl(CtlType type, std::any&& value) : type_(type), value_(std::move(value)) {}

void Ctl::evaluate(Transaction& t) const {
  switch (type_) {
  case CtlType::AuditEngine:
    evaluate_audit_engine(t);
    break;
  case CtlType::AuditLogParts:
    evaluate_audit_log_parts(t);
    break;
  case CtlType::RequestBodyAccess:
    evaluate_request_body_access(t);
    break;
  case CtlType::RequestBodyProcessor:
    evaluate_request_body_processor(t);
    break;
  case CtlType::RuleEngine:
    evaluate_rule_engine(t);
    break;
  case CtlType::RuleRemoveById:
    evaluate_rule_remove_by_id(t);
    break;
  case CtlType::RuleRemoveByTag:
    evaluate_rule_remove_by_tag(t);
    break;
  case CtlType::RuleRemoveTargetById:
    evaluate_rule_remove_target_by_id(t);
    break;
  case CtlType::RuleRemoveTargetByTag:
    evaluate_rule_remove_target_by_tag(t);
    break;
  default:
    UNREACHABLE();
    break;
  }
}

void Ctl::evaluate_audit_engine(Transaction& t) const {
  UNREACHABLE();
  throw "Not implemented!";
}

void Ctl::evaluate_audit_log_parts(Transaction& t) const {
  UNREACHABLE();
  throw "Not implemented!";
}

void Ctl::evaluate_request_body_access(Transaction& t) const {
  UNREACHABLE();
  throw "Not implemented!";
}

void Ctl::evaluate_request_body_processor(Transaction& t) const {
  t.setRequestBodyProcessor(std::any_cast<BodyProcessorType>(value_));
}

void Ctl::evaluate_rule_engine(Transaction& t) const {
  UNREACHABLE();
  throw "Not implemented!";
}

void Ctl::evaluate_rule_remove_by_id(Transaction& t) const {
  UNREACHABLE();
  throw "Not implemented!";
}

void Ctl::evaluate_rule_remove_by_tag(Transaction& t) const {
  UNREACHABLE();
  throw "Not implemented!";
}

void Ctl::evaluate_rule_remove_target_by_id(Transaction& t) const {
  UNREACHABLE();
  throw "Not implemented!";
}

void Ctl::evaluate_rule_remove_target_by_tag(Transaction& t) const {
  UNREACHABLE();
  throw "Not implemented!";
}

} // namespace Action
} // namespace SrSecurity