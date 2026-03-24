+++
title = "RESPONSE_CONTENT_LENGTH"
weight = 29
+++

**Description:** Response content length

**Syntax:** `RESPONSE_CONTENT_LENGTH`

This variable returns the `content-length` response header and is equivalent to `RESPONSE_HEADERS:content-length`.

**Example:**

```apache
SecRule RESPONSE_CONTENT_LENGTH "@rx value" "id:1001,deny,msg:'Test'"
```
