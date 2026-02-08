+++
title = "RESPONSE_HEADERS"
weight = 25
+++

**Description:** Response headers

**Syntax:** `RESPONSE_HEADERS | RESPONSE_HEADERS:Key`

**Implementation Status:** No

**Example:**

```apache
SecRule RESPONSE_HEADERS:User-Agent "@rx box" "id:1001,deny,msg:'Test'"
```
