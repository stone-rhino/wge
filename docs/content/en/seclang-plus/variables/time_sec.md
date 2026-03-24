+++
title = "TIME_SEC"
weight = 100
+++

**Description:** Current second value

**Syntax:** `TIME_SEC`

**Implementation Status:** No

WGE can parse this directive, but the functionality is not implemented yet.

**Example:**

```apache
SecRule TIME_SEC "@rx value" "id:1001,deny,msg:'Test'"
```
