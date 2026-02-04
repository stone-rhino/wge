+++
title = "SERVER_PORT"
weight = 37
+++

**Description:** Server port

**Syntax:** `SERVER_PORT`

**Example:**

```apache
SecRule SERVER_PORT "@rx value" "id:1001,deny,msg:'Test'"
```

**Implementation Status:** Not yet implemented
