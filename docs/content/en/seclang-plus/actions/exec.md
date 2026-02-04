+++
title = "exec"
weight = 32
+++

**Description:** Execute external script or command

**Syntax:** `exec:/path/to/script`

The exec action executes an external script or command when the rule matches. This can be used for custom logging, alerting, or integration with external systems. The script is executed asynchronously to avoid blocking request processing.

**Example:**

```apache
SecRule ARGS "@detectSQLi" "id:1001,deny,exec:/usr/local/bin/alert_admin.sh"
```

**Parameter Type:** `string (file path)`

**Case Sensitive:** Yes
