+++
title = "TIME_DAY"
weight = 97
+++

**Description:** Day of the month

**Syntax:** `TIME_DAY`

**Implementation Status:** No

WGE can parse this directive, but the functionality is not implemented yet.

**Example:**

```apache
SecRule TIME_DAY "@rx value" "id:1001,deny,msg:'Test'"
```
