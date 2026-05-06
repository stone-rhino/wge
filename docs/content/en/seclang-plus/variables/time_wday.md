+++
title = "TIME_WDAY"
weight = 101
+++

**Description:** Current weekday value

**Syntax:** `TIME_WDAY`

**Implementation Status:** No

WGE can parse this directive, but the functionality is not implemented yet.

**Example:**

```apache
SecRule TIME_WDAY "@rx value" "id:1001,deny,msg:'Test'"
```
