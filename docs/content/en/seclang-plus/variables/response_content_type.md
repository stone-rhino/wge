+++
title = "RESPONSE_CONTENT_TYPE"
weight = 30
+++

**Description:** Response content type

**Syntax:** `RESPONSE_CONTENT_TYPE`

This variable returns the `content-type` response header and is equivalent to `RESPONSE_HEADERS:content-type`.

**Example:**

```apache
SecRule RESPONSE_CONTENT_TYPE "@rx value" "id:1001,deny,msg:'Test'"
```
