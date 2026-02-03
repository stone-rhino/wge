+++
title = "RESPONSE_STATUS"
weight = 27
+++

**Description:** Response status code

**Syntax:** `RESPONSE_STATUS`

RESPONSE_STATUS contains the HTTP status code returned by the server, such as 200, 404, 500, etc. Used to detect abnormal responses or implement security policies based on status codes. Available in phase:3 (response headers phase) or later.

**Example:**

```apache
# Log all 5xx errors
SecRule RESPONSE_STATUS "@rx ^5" \
    "id:1036,phase:3,pass,log,msg:'Server error: %{RESPONSE_STATUS}'"

# Detect information leakage (some applications may include debug info with 500 responses)
SecRule RESPONSE_STATUS "@eq 500" \
    "id:1037,phase:3,pass,log,msg:'Internal server error detected'"
```
