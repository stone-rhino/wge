+++
title = "RESPONSE_HEADERS_NAMES"
weight = 26
+++

**Description:** Response header names

**Syntax:** `RESPONSE_HEADERS_NAMES | RESPONSE_HEADERS_NAMES:Key`

This variable references response header names and behaves the same way as request header names in `REQUEST_HEADERS_NAMES`.

**Example:**

```apache
SecRule RESPONSE_HEADERS_NAMES:User-Agent "@rx box" "id:1001,deny,msg:'Test'"
```
