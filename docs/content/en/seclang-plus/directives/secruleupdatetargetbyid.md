+++
title = "SecRuleUpdateTargetById"
weight = 44
+++

**Description:** Update a rule's inspection targets by ID.

**Syntax:** `SecRuleUpdateTargetById ID VARIABLES`

This directive will use the targets provided in the second parameter to append (or replace) variables in the specified rule's current target list.

This functionality is useful for implementing exceptions where targets need to be externally updated to exclude inspection of specific variables.

Note that regular expressions can also be used in target specifications. If you need to use grouping in the regular expression, you must enclose it in single quotes.

You can also replace targets with options more suitable for your environment. For example, if you need to check REQUEST_URI instead of REQUEST_FILENAME, you can do so as follows.

**Example:**

```apache
SecRuleUpdateTargetById 1001 !ARGS:username
```

**Case Sensitive:** Yes
