+++
title = "@lt"
weight = 15
+++

**Description:** Less than

**Syntax:** `"@lt number"`

@lt checks if the variable value (converted to integer) is less than the specified number. Commonly used for detecting insufficient values such as response status code range detection and minimum length validation scenarios. Can be combined with @gt for range detection.

**Example:**

```apache
# Detect successful response (status code < 400)
SecRule RESPONSE_STATUS "@lt 400" \
    "id:1092,phase:3,pass,nolog,setvar:tx.success_response=1"

# Detect minimum content length
SecRule REQUEST_HEADERS:Content-Length "@lt 10" \
    "id:1093,phase:1,pass,nolog,setvar:tx.small_body=1"
```

**Parameter Type:** `int`

**Case Sensitive:** Yes
