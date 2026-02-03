+++
title = "REMOTE_USER"
weight = 34
+++

**Description:** Authenticated username

**Syntax:** `REMOTE_USER`

**Example:**

```apache
SecRule REMOTE_USER "@rx value" "id:1001,deny,msg:'Test'"
```

**Implementation Status:** Not yet implemented
