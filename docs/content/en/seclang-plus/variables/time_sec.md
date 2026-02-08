+++
title = "TIME_SEC"
weight = 100
+++

**Description:** Second

**Syntax:** `TIME_SEC`

**Implementation Status:** No

**Example:**

```apache
SecRule TIME_SEC "@rx value" "id:1001,deny,msg:'Test'"
```
