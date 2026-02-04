+++
title = "REMOTE_HOST"
weight = 32
+++

**Description:** Client hostname

**Syntax:** `REMOTE_HOST`

**Example:**

```apache
SecRule REMOTE_HOST "@rx value" "id:1001,deny,msg:'Test'"
```

**Implementation Status:** Not yet implemented
