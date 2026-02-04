+++
title = "@detectXSS"
weight = 30
+++

**Description:** Detects Cross-Site Scripting (XSS) attacks using the libinjection library

**Syntax:** `"@detectXSS"`

@detectXSS uses libinjection's XSS detection engine to detect XSS attacks by identifying HTML and JavaScript injection patterns. It can detect various types of XSS including reflected, stored, and DOM-based XSS. This operator requires no parameters.

**Example:**

```apache
# Detect XSS attacks in parameters
SecRule ARGS "@detectXSS" \
    "id:1001,phase:2,deny,msg:'XSS Attack Detected'"

# Detect XSS in request body
SecRule REQUEST_BODY "@detectXSS" \
    "id:1002,phase:2,deny,severity:CRITICAL,\
    msg:'XSS Attack in request body'"
```

**Parameter Type:** `none`
