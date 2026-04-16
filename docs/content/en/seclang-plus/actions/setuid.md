+++
title = "setuid"
weight = 28
+++

**Description:** Initialize the `USER` collection using the provided parameter

**Syntax:** `setuid:('string' | %{variable})`

**Case Sensitive:** Yes

**Implementation Status:** Not yet implemented

This action supports execution on non-match and unconditional execution.

WGE can parse this directive correctly, but the feature is not implemented yet.

**Example:**

```apache
SecRule ARGS:username ".*" "phase:2,id:137,t:none,pass,capture,setuid:%{TX.0}"

# Execute on non-match
SecRule ARGS:username ".*" "phase:2,id:138,t:none,pass,capture,!setuid:%{TX.0}"

# Always execute
SecRule ARGS:username ".*" "phase:2,id:139,t:none,pass,capture,*setuid:%{TX.0}"
```
