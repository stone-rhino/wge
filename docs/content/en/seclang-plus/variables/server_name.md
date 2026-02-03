+++
title = "SERVER_NAME"
weight = 36
+++

**Description:** Server name

**Syntax:** `SERVER_NAME`

**Example:**

```apache
SecRule SERVER_NAME "@rx value" "id:1001,deny,msg:'Test'"
```

**Implementation Status:** Not yet implemented
