+++
title = "REQUEST_URI"
weight = 15
+++

**Description:** Request URI (decoded)

**Syntax:** `REQUEST_URI`

REQUEST_URI contains the complete request URI, including the query string portion, and is URL-decoded. This is the primary variable for detecting attack payloads in URLs. Unlike REQUEST_URI_RAW, the content of this variable is decoded, making it easier to match attack patterns directly.

**Example:**

```apache
# Detect directory traversal attacks in URL
SecRule REQUEST_URI "@rx \.\." \
    "id:1020,phase:1,deny,msg:'Directory traversal attack detected'"

# Block access to sensitive files
SecRule REQUEST_URI "@rx \.(conf|ini|log|bak)$" \
    "id:1021,phase:1,deny,msg:'Attempt to access sensitive file'"
```
