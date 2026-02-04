+++
title = "REQUEST_HEADERS_NAMES"
weight = 9
+++

**Description:** Request header names

**Syntax:** `REQUEST_HEADERS_NAMES`

REQUEST_HEADERS_NAMES contains the names of all request headers (excluding values). Used to detect abnormal custom headers or whether header names themselves contain attack payloads.

**Example:**

```apache
# Detect abnormal characters in header names
SecRule REQUEST_HEADERS_NAMES "@rx [<>'\"]" \
    "id:1012,phase:1,deny,msg:'Request header name contains illegal characters'"
```
