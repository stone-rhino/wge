+++
title = "TIME_MIN"
weight = 99
+++

**Description:** Current minute value

**Syntax:** `TIME_MIN`

**Implementation Status:** No

WGE can parse this directive, but the functionality is not implemented yet.

**Example:**

```apache
SecRule TIME_MIN "@rx value" "id:1001,deny,msg:'Test'"
```
