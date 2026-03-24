+++
title = "RESPONSE_HEADERS"
weight = 25
+++

**Description:** Response headers

**Syntax:** `RESPONSE_HEADERS | RESPONSE_HEADERS:Key`

This variable references response header values and behaves the same way as request headers in `REQUEST_HEADERS`.

**Example:**

```apache
SecRule RESPONSE_HEADERS:User-Agent "@rx box" "id:1001,deny,msg:'Test'"
```
