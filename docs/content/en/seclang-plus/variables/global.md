+++
title = "GLOBAL"
weight = 82
+++

**Description:** Global variable collection

**Syntax:** `GLOBAL:key`

The GLOBAL collection is used to store global persistent data across transactions. Unlike TX, GLOBAL variables persist across multiple requests and can be used to implement global counters, global status flags, etc. Must be initialized via the initcol action before use.

This variable is not recommended at the moment because its parsing and assignment behavior still has some issues.

**Example:**

```apache
# Initialize global collection
SecAction "id:1055,phase:1,pass,nolog,initcol:GLOBAL=global"

# Global request count
SecRule GLOBAL "@eq 100" "id:1056,phase:1,deny,nolog"
```
