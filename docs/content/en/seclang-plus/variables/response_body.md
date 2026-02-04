+++
title = "RESPONSE_BODY"
weight = 24
+++

**Description:** Response body content

**Syntax:** `RESPONSE_BODY`

RESPONSE_BODY contains the server's response body content. SecResponseBodyAccess must be enabled to access this variable. Used to detect sensitive information leakage, malicious content, or abnormal patterns in responses. Use in phase:4 (response body phase).

**Example:**

```apache
# Detect credit card number leakage in response
SecRule RESPONSE_BODY "@rx \b(?:\d{4}[-\s]?){3}\d{4}\b" \
    "id:1034,phase:4,deny,msg:'Credit card number leakage detected'"

# Detect SQL error message leakage
SecRule RESPONSE_BODY "@pm mysql_error ora-00 sql syntax error" \
    "id:1035,phase:4,deny,msg:'Database error message leakage detected'"
```
