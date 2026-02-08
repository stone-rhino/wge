+++
title = "AUTH_TYPE"
weight = 38
+++

**Description:** Authentication type

**Syntax:** `AUTH_TYPE`

**Implementation Status:** No

**Example:**

```apache
SecRule AUTH_TYPE "@rx value" "id:1001,deny,msg:'Test'"
```
