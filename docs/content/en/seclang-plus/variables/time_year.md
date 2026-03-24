+++
title = "TIME_YEAR"
weight = 95
+++

**Description:** Current year value

**Syntax:** `TIME_YEAR`

**Implementation Status:** No

WGE can parse this directive, but the functionality is not implemented yet.

**Example:**

```apache
SecRule TIME_YEAR "@rx value" "id:1001,deny,msg:'Test'"
```
