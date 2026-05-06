+++
title = "SecRuleRemoveByTag"
weight = 42
+++

**Description:** Remove rules by tag.


**Syntax:** `SecRuleRemoveByTag "STRING"`


**Case Sensitive:** Yes



Typically SecRuleRemoveById is used to remove rules, but sometimes it is more convenient to disable a group of rules using SecRuleRemoveByTag. Matching is case-sensitive.

Like SecRuleRemoveById, it can only disable rules that appear before it.

**Example:**


```apache
SecRuleRemoveByTag "attack-sqli"
```
