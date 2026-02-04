+++
title = "SecRuleRemoveById"
weight = 40
+++

**Description:** Remove rules by ID.

**Syntax:** `SecRuleRemoveById ID [ID_RANGE] ...`

This directive supports multiple parameters, each of which can be a rule ID or range.

**Example:**

```apache
# Remove a single rule
SecRuleRemoveById 1001

# Remove multiple rules
SecRuleRemoveById 1001 1002 1003

# Remove a range of rules
SecRuleRemoveById 1001-1100
```

**Case Sensitive:** Yes
