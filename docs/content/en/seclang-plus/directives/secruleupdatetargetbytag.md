+++
title = "SecRuleUpdateTargetByTag"
weight = 46
+++

**Description:** Update a rule's variable list by tag.


**Syntax:** `SecRuleUpdateTargetByTag "TAG" VARIABLES`


**Case Sensitive:** Yes

Similar to SecRuleUpdateTargetById, SecRuleUpdateTargetByTag modifies the variables of specified rules by matching the tag. For more details, see SecRuleUpdateTargetById.


**Example:**


```apache
SecRuleUpdateTargetByTag "attack-sqli" !ARGS:search
```
