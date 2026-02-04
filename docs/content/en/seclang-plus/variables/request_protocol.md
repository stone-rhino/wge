+++
title = "REQUEST_PROTOCOL"
weight = 19
+++

**Description:** Request protocol (HTTP/1.1, etc.)

**Syntax:** `REQUEST_PROTOCOL`

REQUEST_PROTOCOL contains the HTTP protocol version used in the request, such as HTTP/1.0, HTTP/1.1, or HTTP/2.0. Used to detect protocol violations or enforce specific protocol versions.

**Example:**

```apache
# Reject HTTP/1.0 requests
SecRule REQUEST_PROTOCOL "@streq HTTP/1.0" \
    "id:1028,phase:1,deny,msg:'HTTP/1.0 not supported'"

# Detect abnormal protocol
SecRule REQUEST_PROTOCOL "!@rx ^HTTP/[12]\.[0-9]$" \
    "id:1029,phase:1,deny,msg:'Invalid HTTP protocol'"
```
