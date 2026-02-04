+++
title = "SecRuleUpdateTargetByMsg"
weight = 45
+++

**Description:** Update rule inspection targets by message content.

**Syntax:** `SecRuleUpdateTargetByMsg "MESSAGE" VARIABLES`

This directive will use the targets provided in the second parameter to append (or replace) variables in the specified rule's current target list.

This approach is useful for implementing exceptions where targets need to be externally updated to exclude inspection of specific variables.

You can also completely replace the target list to make it more suitable for your environment. For example, if you need to check REQUEST_URI instead of REQUEST_FILENAME, you can do so as follows.

**Example:**

```apache
SecRuleUpdateTargetByMsg "SQL Injection" !ARGS:id
```

**Case Sensitive:** Yes
