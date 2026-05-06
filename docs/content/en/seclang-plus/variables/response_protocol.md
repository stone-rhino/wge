+++
title = "RESPONSE_PROTOCOL"
weight = 28
+++

**Description:** Response protocol

**Syntax:** `RESPONSE_PROTOCOL`

This variable contains HTTP response protocol information.

**Example:**

```apache
SecRule RESPONSE_PROTOCOL "@rx value" "id:1001,deny,msg:'Test'"
```
