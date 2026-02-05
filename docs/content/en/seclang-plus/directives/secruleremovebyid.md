+++
title = "SecRuleRemoveById"
weight = 40
+++

**Description:** Remove rules by ID.


**Syntax:** `SecRuleRemoveById [ID|ID_RANGE] ...`


**Case Sensitive:** Yes



This directive supports multiple parameters, each of which can be a rule ID or range.

**Note:** SecRuleRemoveById should be loaded last; it will not take effect if it appears before the rules it intends to disable.

**Example:**


```apache
# Remove a single rule
SecRuleRemoveById 1001

# Remove multiple rules
SecRuleRemoveById 1001 1002 1003

# Remove a range of rules
SecRuleRemoveById 1001-1100
```
