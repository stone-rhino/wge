+++
title = "RESPONSE_CONTENT_TYPE"
weight = 30
+++

**Description:** Response content type

**Syntax:** `RESPONSE_CONTENT_TYPE`

**Implementation Status:** No

**Example:**

```apache
SecRule RESPONSE_CONTENT_TYPE "@rx value" "id:1001,deny,msg:'Test'"
```
