+++
title = "@le"
weight = 17
+++

**Description:** Less than or equal to

**Syntax:** `"@le number"`

@le checks if the variable value (converted to integer) is less than or equal to the specified number. Commonly used for upper limit detection such as response status code range checks and request count limits within time windows. Can be combined with @ge for closed-interval range detection.

**Example:**

```apache
# Detect normal response (status code <= 399)
SecRule RESPONSE_STATUS "@le 399" \
    "id:1098,phase:3,pass,nolog,setvar:tx.normal_response=1"

# Detect requests within rate limit
SecRule IP:request_count "@le 100" \
    "id:1099,phase:1,pass,nolog"
```

**Parameter Type:** `int`

**Case Sensitive:** Yes
