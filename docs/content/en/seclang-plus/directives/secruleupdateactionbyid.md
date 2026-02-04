+++
title = "SecRuleUpdateActionById"
weight = 43
+++

**Description:** Update a rule's actions by ID.

**Syntax:** `SecRuleUpdateActionById ID "ACTIONS"`

This directive will override the specified rule's action list with the actions provided in the second parameter. There are two limitations: it cannot be used to modify the rule ID or phase; it only overrides actions that can only appear once. Actions that are allowed to appear multiple times in the list will be appended to the end.

Adding t:none will override any previously specified transformation functions (such as t:lowercase in the example).

**Example:**

```apache
SecRuleUpdateActionById 1001 "pass,log"
```

**Case Sensitive:** Yes
