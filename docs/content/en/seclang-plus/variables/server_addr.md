+++
title = "SERVER_ADDR"
weight = 35
+++

**Description:** Server IP address

**Syntax:** `SERVER_ADDR`

**Example:**

```apache
SecRule SERVER_ADDR "@rx value" "id:1001,deny,msg:'Test'"
```

**Implementation Status:** Not yet implemented
