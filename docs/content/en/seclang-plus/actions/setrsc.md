+++
title = "setrsc"
weight = 30
+++

**Description:** Special action that initializes the `RESOURCE` collection with the provided key

**Syntax:** `setrsc:('string' | %{variable})`

**Case Sensitive:** Yes

**Implementation Status:** Not yet implemented

This action supports execution on non-match and unconditional execution.

WGE can parse this directive correctly, but the feature is not implemented yet.

**Example:**

```apache
SecRule ARGS "@rx test" "id:1001,setrsc:'abcd1234',status:403"

# Execute on non-match
SecRule ARGS "@rx test" "id:1002,!setrsc:'abcd1234',status:403"

# Always execute
SecRule ARGS "@rx test" "id:1003,*setrsc:'abcd1234',status:403"
```
