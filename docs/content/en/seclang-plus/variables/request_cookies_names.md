+++
title = "REQUEST_COOKIES_NAMES"
weight = 11
+++

**Description:** Cookie names

**Syntax:** `REQUEST_COOKIES_NAMES`

REQUEST_COOKIES_NAMES contains the names of all cookies. Used to detect whether cookie names contain abnormal characters or attack payloads.

**Example:**

```apache
# Detect abnormal characters in cookie names
SecRule REQUEST_COOKIES_NAMES "@rx [<>'\";()]" \
    "id:1015,phase:1,deny,msg:'Cookie name contains illegal characters'"
```
