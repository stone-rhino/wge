+++
title = "IP"
weight = 83
+++

**Description:** IP address collection

**Syntax:** `IP:key`

The IP collection is used to store persistent data associated with client IP addresses. Commonly used to implement IP-level request rate limiting, IP reputation scoring, etc. Needs to be initialized via initcol:IP=%{REMOTE_ADDR}. Data in the IP collection persists across multiple requests from the same client IP.

**Example:**

```apache
# Initialize IP collection
SecAction "id:1057,phase:1,pass,nolog,initcol:IP=%{REMOTE_ADDR}"

# IP-level request rate limiting
SecRule IP:request_count "@gt 100" \
    "id:1058,phase:1,deny,msg:'IP request rate exceeded'"
SecRule REQUEST_URI "@rx ." \
    "id:1059,phase:1,pass,nolog,setvar:IP.request_count=+1,\
    expirevar:IP.request_count=60"
```
