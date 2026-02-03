+++
title = "@validateUrlEncoding"
weight = 22
+++

**Description:** Validate URL encoding

**Syntax:** `"@validateUrlEncoding"`

@validateUrlEncoding checks if URL encoding (percent-encoding) in the variable value is valid. Matches if invalid URL encoding is found. This operator detects attacks using invalid URL encoding for detection evasion, such as malformed encoding like %ZZ or %1. Requires no parameters.

**Example:**

```apache
# Detect invalid URL encoding
SecRule ARGS "@validateUrlEncoding" \
    "id:1113,phase:2,deny,msg:'Invalid URL encoding detected'"

# Detect invalid encoding in URI
SecRule REQUEST_URI "@validateUrlEncoding" \
    "id:1114,phase:1,deny,msg:'Invalid URL encoding in URI'"

# Detect encoding issues in query string
SecRule QUERY_STRING "@validateUrlEncoding" \
    "id:1115,phase:1,deny,msg:'Invalid encoding in query string'"
```

**Parameter Type:** `none`

**Case Sensitive:** Yes
