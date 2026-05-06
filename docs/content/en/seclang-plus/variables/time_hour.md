+++
title = "TIME_HOUR"
weight = 98
+++

**Description:** Current hour value

**Syntax:** `TIME_HOUR`

**Implementation Status:** No

WGE can parse this directive, but the functionality is not implemented yet.

**Example:**

```apache
SecRule TIME_HOUR "@rx value" "id:1001,deny,msg:'Test'"
```
