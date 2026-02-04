+++
title = "RESPONSE_HEADERS"
weight = 25
+++

**Description:** Response headers

**Syntax:** `RESPONSE_HEADERS | RESPONSE_HEADERS:Key`

**Example:**

```apache
SecRule RESPONSE_HEADERS:User-Agent "@rx box" "id:1001,deny,msg:'Test'"
```

**Implementation Status:** Not yet implemented
