+++
title = "TIME_MON"
weight = 96
+++

**Description:** Current month value

**Syntax:** `TIME_MON`

**Implementation Status:** No

WGE can parse this directive, but the functionality is not implemented yet.

**Example:**

```apache
SecRule TIME_MON "@rx value" "id:1001,deny,msg:'Test'"
```
