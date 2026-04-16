+++
title = "setsid"
weight = 29
+++

**Description:** Initialize the `SESSION` collection using the provided parameter

**Syntax:** `setsid:('string' | %{variable})`

**Case Sensitive:** Yes

**Implementation Status:** Not yet implemented

This action supports execution on non-match and unconditional execution.

WGE can parse this directive correctly, but the feature is not implemented yet.

**Example:**

```apache
SecRule ARGS "@rx test" "id:1001,phase:2,setsid:%{REQUEST_COOKIES.PHPSESSID},status:403"

# Execute on non-match
SecRule ARGS "@rx test" "id:1002,phase:2,!setsid:%{REQUEST_COOKIES.PHPSESSID},status:403"

# Always execute
SecRule ARGS "@rx test" "id:1003,phase:2,*setsid:%{REQUEST_COOKIES.PHPSESSID},status:403"
```
