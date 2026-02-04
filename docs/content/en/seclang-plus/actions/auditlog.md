+++
title = "auditlog"
weight = 20
+++

**Description:** Log to audit log

**Syntax:** `auditlog | auditlog:value`

The auditlog action forces the transaction to be recorded in the audit log. The audit log contains detailed information about the request and response, which is useful for forensic analysis and compliance purposes.

**Example:**

```apache
SecRule ARGS "@rx test" "id:1001,auditlog,status:403"
```

**Parameter Type:** `none`

**Case Sensitive:** Yes
