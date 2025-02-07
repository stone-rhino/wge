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

  enum class BodyProcessorType { UrlEncoded, MultiPart, Xml, Json };

public:
  Ctl(CtlType type, std::any&& value);

public:
  void evaluate(Transaction& t) override;

private:
  CtlType type_;
  std::any value_;
};
} // namespace Action
} // namespace SrSecurity