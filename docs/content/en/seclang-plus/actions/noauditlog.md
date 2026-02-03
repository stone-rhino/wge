+++
title = "noauditlog"
weight = 21
+++

**Description:** Do not log to audit log

**Syntax:** `noauditlog | noauditlog:value`

The noauditlog action prevents the transaction from being recorded in the audit log even when a rule matches. This is useful for rules that match frequently on known-benign traffic and would otherwise fill up the audit log with noise.

**Example:**

```apache
SecRule ARGS "@rx test" "id:1001,noauditlog,status:403"
```

**Parameter Type:** `none`

**Case Sensitive:** Yes
