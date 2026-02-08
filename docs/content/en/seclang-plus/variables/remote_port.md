+++
title = "REMOTE_PORT"
weight = 33
+++

**Description:** Client port

**Syntax:** `REMOTE_PORT`

**Implementation Status:** No

**Example:**

```apache
SecRule REMOTE_PORT "@rx value" "id:1001,deny,msg:'Test'"
```
