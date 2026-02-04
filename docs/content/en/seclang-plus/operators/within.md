+++
title = "@within"
weight = 5
+++

**Description:** Value is within the specified string

**Syntax:** `"@within string"`

@within is the reverse of @contains: checks if the variable value exists as a substring within the specified parameter string. In other words, it checks if the parameter string contains the variable value. Matching is case sensitive. This is particularly useful for whitelist checks to verify if a value is in the allowed list.

**Example:**

```apache
# Validate HTTP method is in allowed list
SecRule REQUEST_METHOD "!@within GET POST HEAD OPTIONS" \
    "id:1078,phase:1,deny,msg:'Disallowed HTTP method'"

# Validate file extension is in whitelist
SecRule FILES_COMBINED_SIZE "@gt 0" \
    "id:1079,phase:2,chain"
    SecRule FILES_NAMES "!@within .jpg .png .gif .pdf" \
        "deny,msg:'Disallowed file type'"

# Validate Content-Type is legitimate
SecRule REQUEST_HEADERS:Content-Type "!@within application/json application/xml text/plain" \
    "id:1080,phase:1,deny,msg:'Unsupported Content-Type'"
```

**Parameter Type:** `string`

**Case Sensitive:** Yes
