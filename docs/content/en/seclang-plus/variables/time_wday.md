+++
title = "TIME_WDAY"
weight = 101
+++

**Description:** Day of the week

**Syntax:** `TIME_WDAY`

**Implementation Status:** No

**Example:**

```apache
SecRule TIME_WDAY "@rx value" "id:1001,deny,msg:'Test'"
```
