+++
title = "TIME_EPOCH"
weight = 94
+++

**Description:** Unix timestamp

**Syntax:** `TIME_EPOCH`

**Implementation Status:** No

WGE can parse this directive, but the functionality is not implemented yet.

**Example:**

```apache
SecRule TIME_EPOCH "@rx value" "id:1001,deny,msg:'Test'"
```
