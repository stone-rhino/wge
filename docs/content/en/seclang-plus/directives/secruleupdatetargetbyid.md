+++
title = "SecRuleUpdateTargetById"
weight = 44
+++

**Description:** Update a rule's variable list by ID.


**Syntax:** `SecRuleUpdateTargetById ID VARIABLES`


**Case Sensitive:** Yes

Updates the variables (VARIABLES) of the rule with the specified ID, allowing operations such as adding or removing variables.

**Note:** This directive is a configuration directive, not runtime - meaning skipAfter and similar directives do not affect it. It must be loaded after the rule it updates.

**Example:**

If the rule with ID 1001 is:

```apache
SecRule ARGS|REQUEST_HEADERS "@rx admin" "id:1001,phase:1,deny,tag:'template_rule',t:none"
```

**Add variable:**

The following example will add REQUEST_LINE detection to rule 1001:
```apache
SecRuleUpdateTargetById 1001 REQUEST_LINE
```

**Remove variable:**

The following example will remove ARGS detection from rule 1001:
```apache
SecRuleUpdateTargetById 1001 !ARGS
```

**Remove specific sub-variable:**

The following example will make rule 1001 not inspect the parameter named username:
```apache
SecRuleUpdateTargetById 1001 !ARGS:username
```

**Using regex:**

The following example will make rule 1001 not inspect parameters matching the specified regex:
```apache
SecRuleUpdateTargetById 1001 !ARGS:/[a-zA-Z]{100,}/
```
