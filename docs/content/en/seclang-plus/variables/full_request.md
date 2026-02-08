+++
title = "FULL_REQUEST"
weight = 22
+++

**Description:** Complete request data

**Syntax:** `FULL_REQUEST`

**Implementation Status:** No

**Example:**

```apache
SecRule FULL_REQUEST "@rx value" "id:1001,deny,msg:'Test'"
```
