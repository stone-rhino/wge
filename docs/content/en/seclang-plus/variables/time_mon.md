+++
title = "TIME_MON"
weight = 96
+++

**Description:** Month

**Syntax:** `TIME_MON`

**Implementation Status:** No

**Example:**

```apache
SecRule TIME_MON "@rx value" "id:1001,deny,msg:'Test'"
```
