+++
title = "SecRuleUpdateTargetByMsg"
weight = 45
+++

**Description:** Update a rule's variable list by message content.


**Syntax:** `SecRuleUpdateTargetByMsg "MESSAGE" VARIABLES`


**Case Sensitive:** Yes

Similar to SecRuleUpdateTargetById, SecRuleUpdateTargetByMsg modifies the variables of specified rules by matching the msg. For more details, see SecRuleUpdateTargetById.

**Example:**


```apache
SecRuleUpdateTargetByMsg "SQL Injection" !ARGS:id
```
