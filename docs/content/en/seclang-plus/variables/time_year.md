+++
title = "TIME_YEAR"
weight = 95
+++

**Description:** Year

**Syntax:** `TIME_YEAR`

**Implementation Status:** No

**Example:**

```apache
SecRule TIME_YEAR "@rx value" "id:1001,deny,msg:'Test'"
```
