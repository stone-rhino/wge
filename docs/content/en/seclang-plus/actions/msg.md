+++
title = "msg"
weight = 3
+++

**Description:** Log message

**Syntax:** `msg:'value'`

**Case Sensitive:** Yes

Similar to `logdata`, `msg` also supports macro expansion.

**Example:**

```apache
SecRule ARGS "@rx test" "id:1001,deny,msg:'SQL Injection Detected'"

# msg with macro expansion
SecRule ARGS "@rx test2" "id:1002,deny,msg:'Deny the ip %{REMOTE_ADDR}'"
```
