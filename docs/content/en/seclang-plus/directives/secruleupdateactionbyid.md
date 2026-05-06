+++
title = "SecRuleUpdateActionById"
weight = 43
+++

**Description:** Update a rule's actions by ID.


**Syntax:** `SecRuleUpdateActionById INT|(INT ':' '-'? INT) "ACTIONS"`


**Case Sensitive:** Yes



This directive will override the specified rule's action list with the actions provided in the second parameter. There are two limitations: it cannot be used to modify the rule ID or phase; it only overrides actions that can only appear once. Actions that are allowed to appear multiple times in the list will be appended to the end.

Adding t:none will override any previously specified transformation functions (such as t:lowercase in the example).

For rules with chained rules, you can use SecRuleUpdateActionById id:0 to update the action of chained rules. The second INT value of -1 represents the top-level parent rule, 0 represents the child rule at depth 1, and so on.

**Example:**


```apache
SecRuleUpdateActionById 1001 "pass,log"
```
