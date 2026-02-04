+++
title = "initcol"
weight = 31
+++

**Description:** Initialize persistent collection

**Syntax:** `initcol:collection=key`

The initcol action initializes persistent collections (IP, SESSION, USER, RESOURCE, GLOBAL). Persistent collections allow saving and sharing data across multiple requests, forming the foundation for implementing IP rate limiting, session tracking, user behavior analysis, and similar features.

**Available Collections:**

- **IP** - Persistent storage based on client IP address
- **SESSION** - Persistent storage based on session ID
- **USER** - Persistent storage based on user ID
- **RESOURCE** - Persistent storage based on resource identifier
- **GLOBAL** - Global persistent storage

Collection data is automatically loaded from storage during rule execution and automatically persisted after modification.

**Example:**

```apache
# Initialize IP collection for rate limiting
SecAction "id:1,phase:1,pass,nolog,initcol:IP=%{REMOTE_ADDR}"

# IP rate limiting implementation
SecRule IP:request_count "@gt 100" \
    "id:2,phase:1,deny,msg:'IP request rate exceeded'"
SecRule REQUEST_URI "@unconditionalMatch" \
    "id:3,phase:1,pass,nolog,setvar:IP.request_count=+1,\
    expirevar:IP.request_count=60"

# Initialize session collection
SecRule REQUEST_COOKIES:session_id "@rx ^[a-f0-9]{32}$" \
    "id:4,phase:1,pass,nolog,initcol:SESSION=%{REQUEST_COOKIES.session_id}"

# Initialize global collection
SecAction "id:5,phase:1,pass,nolog,initcol:GLOBAL=global"
```

**Parameter Type:** `string`

**Case Sensitive:** Yes
