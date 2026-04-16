+++
title = "setenv"
weight = 27
+++

**Description:** Set Linux/Unix environment variable (overwrite if it already exists)

**Info:** Original example: `setenv:attack_detected=1`

**Syntax:** `setenv:'string=(value|%{variable})'`

**Case Sensitive:** Yes

This action supports execution on non-match and unconditional execution.

**Example:**

```apache
SecRule ARGS "@rx test" "id:1001,setenv:http_proxy=%{REMOTE_ADDR},status:403"

# Execute on non-match
SecRule ARGS "@rx test" "id:1002,!setenv:http_proxy=%{REMOTE_ADDR},status:403"

# Always execute
SecRule ARGS "@rx test" "id:1003,*setenv:http_proxy=%{REMOTE_ADDR},status:403"
```
