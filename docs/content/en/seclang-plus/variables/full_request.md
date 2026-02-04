+++
title = "FULL_REQUEST"
weight = 22
+++

**Description:** Complete request data

**Syntax:** `FULL_REQUEST`

**Example:**

```apache
SecRule FULL_REQUEST "@rx value" "id:1001,deny,msg:'Test'"
```

**Implementation Status:** Not yet implemented
