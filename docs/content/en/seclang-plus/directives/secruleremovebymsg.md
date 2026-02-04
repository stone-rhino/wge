+++
title = "SecRuleRemoveByMsg"
weight = 41
+++

**Description:** Remove rules by message content.

**Syntax:** `SecRuleRemoveByMsg "MESSAGE"`

Typically SecRuleRemoveById is used to remove rules, but this directive supports removing rules by matching the rule's msg action. Matching uses case-sensitive exact string matching.

**Example:**

```apache
SecRuleRemoveByMsg "SQL Injection Attack"
```

**Case Sensitive:** Yes
