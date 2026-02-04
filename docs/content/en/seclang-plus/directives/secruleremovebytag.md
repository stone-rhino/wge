+++
title = "SecRuleRemoveByTag"
weight = 42
+++

**Description:** Remove rules by tag.

**Syntax:** `SecRuleRemoveByTag "TAG"`

Typically SecRuleRemoveById is used to remove rules, but sometimes it is more convenient to disable a group of rules using SecRuleRemoveByTag. Matching uses case-sensitive string equality.

**Example:**

```apache
SecRuleRemoveByTag "attack-sqli"
```

**Case Sensitive:** Yes
