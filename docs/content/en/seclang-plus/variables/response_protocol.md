+++
title = "RESPONSE_PROTOCOL"
weight = 28
+++

**Description:** Response protocol

**Syntax:** `RESPONSE_PROTOCOL`

**Implementation Status:** No

**Example:**

```apache
SecRule RESPONSE_PROTOCOL "@rx value" "id:1001,deny,msg:'Test'"
```
