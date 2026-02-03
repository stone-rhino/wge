+++
title = "msg"
weight = 3
+++

**Description:** Log message

**Syntax:** `msg:'message'`

The msg action specifies a custom message that will be recorded in the log when the rule matches. This message helps identify the nature of the detected threat and is useful for debugging and security monitoring.

**Example:**

```apache
SecRule ARGS "@rx test" "id:1001,deny,msg:'SQL Injection Detected'"
```

**Parameter Type:** `string`

**Case Sensitive:** Yes
