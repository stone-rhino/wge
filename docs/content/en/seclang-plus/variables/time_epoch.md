+++
title = "TIME_EPOCH"
weight = 94
+++

**Description:** Unix timestamp

**Syntax:** `TIME_EPOCH`

**Example:**

```apache
SecRule TIME_EPOCH "@rx value" "id:1001,deny,msg:'Test'"
```

**Implementation Status:** Not yet implemented
