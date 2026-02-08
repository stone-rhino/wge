+++
title = "TIME_HOUR"
weight = 98
+++

**Description:** Hour

**Syntax:** `TIME_HOUR`

**Implementation Status:** No

**Example:**

```apache
SecRule TIME_HOUR "@rx value" "id:1001,deny,msg:'Test'"
```
