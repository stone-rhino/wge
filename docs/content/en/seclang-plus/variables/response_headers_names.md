+++
title = "RESPONSE_HEADERS_NAMES"
weight = 26
+++

**Description:** Response header names

**Syntax:** `RESPONSE_HEADERS_NAMES | RESPONSE_HEADERS_NAMES:Key`

**Example:**

```apache
SecRule RESPONSE_HEADERS_NAMES:User-Agent "@rx box" "id:1001,deny,msg:'Test'"
```

**Implementation Status:** Not yet implemented
