+++
title = "USERID"
weight = 71
+++

**Description:** User ID

**Syntax:** `USERID`

**Implementation Status:** No

**Example:**

```apache
SecRule USERID "@rx value" "id:1001,deny,msg:'Test'"
```
