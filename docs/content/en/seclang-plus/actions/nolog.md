+++
title = "nolog"
weight = 19
+++

**Description:** Do not log to error log

**Syntax:** `nolog | nolog:value`

The nolog action prevents the rule match from being recorded in the error log. This is typically used for rules that perform internal bookkeeping (such as setting variables) where logging each match would be unnecessary and noisy.

**Example:**

```apache
SecRule ARGS "@rx test" "id:1001,nolog,status:403"
```

**Parameter Type:** `none`

**Case Sensitive:** Yes
