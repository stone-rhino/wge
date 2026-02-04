+++
title = "RESPONSE_CONTENT_LENGTH"
weight = 29
+++

**Description:** Response content length

**Syntax:** `RESPONSE_CONTENT_LENGTH`

**Example:**

```apache
SecRule RESPONSE_CONTENT_LENGTH "@rx value" "id:1001,deny,msg:'Test'"
```

**Implementation Status:** Not yet implemented
