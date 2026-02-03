+++
title = "SecDefaultAction"
weight = 39
+++

**Description:** Define the default action list for a specific phase.

**Syntax:** `SecDefaultAction "ACTIONS"`

**Default:** phase:2,log,auditlog,pass

**Version:** 1.0

In the same configuration context, all rules following a SecDefaultAction directive will inherit its settings unless the rule specifies more specific actions. Each SecDefaultAction directive must specify a disruptive action and processing phase, and cannot contain metadata actions. Note that SecDefaultAction does not inherit across configuration contexts.

**Example:**

```
SecDefaultAction "phase:1,log,auditlog,pass"
SecDefaultAction "phase:2,log,auditlog,deny,status:403"
```

**Case Sensitive:** Yes
