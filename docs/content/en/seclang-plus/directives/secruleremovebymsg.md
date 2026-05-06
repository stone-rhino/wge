+++
title = "SecRuleRemoveByMsg"
weight = 41
+++

**Description:** Remove rules by msg content.


**Syntax:** `SecRuleRemoveByMsg "STRING"`


**Case Sensitive:** Yes



Typically SecRuleRemoveById is used to remove rules, but this directive supports removing rules by matching the rule's msg action. Matching uses case-sensitive exact string matching.

Like SecRuleRemoveById, it can only disable rules that appear before it.

**Example:**


```apache
SecRuleRemoveByMsg "SQL Injection Attack"
```
