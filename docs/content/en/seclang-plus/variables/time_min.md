+++
title = "TIME_MIN"
weight = 99
+++

**Description:** Minute

**Syntax:** `TIME_MIN`

**Implementation Status:** No

**Example:**

```apache
SecRule TIME_MIN "@rx value" "id:1001,deny,msg:'Test'"
```
