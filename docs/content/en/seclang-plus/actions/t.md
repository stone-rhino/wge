+++
title = "t"
weight = 45
+++

**Description:** Apply transformation function

**Syntax:** `t:transformationName`

The t (transformation) action applies a transformation function to the input data before matching against the operator. Transformations normalize or decode data to improve detection accuracy. Multiple transformations can be chained and are applied in order.

Common transformations:
- **t:none** - Disable all default transformations
- **t:lowercase** - Convert to lowercase
- **t:urlDecode** - URL decode
- **t:htmlEntityDecode** - Decode HTML entities
- **t:base64Decode** - Base64 decode
- **t:removeWhitespace** - Remove all whitespace
- **t:compressWhitespace** - Normalize whitespace
- **t:normalizePath** - Normalize path separators
- **t:cmdLine** - Normalize command line
- **t:hexDecode** - Decode hex-encoded data

Important: When using t:none, it should be the first transformation specified to clear inherited transformations from SecDefaultAction.

**Example:**

```apache
# Detect SQL injection with URL decoding and lowercase
SecRule ARGS "@detectSQLi" \
    "id:100,phase:2,deny,t:urlDecode,t:lowercase,msg:'SQL Injection'"

# Clear default transformations before applying custom ones
SecRule ARGS "@rx <script" \
    "id:101,phase:2,deny,t:none,t:htmlEntityDecode,t:lowercase,msg:'XSS'"

# Normalize paths for path traversal detection
SecRule REQUEST_URI "@contains ../" \
    "id:102,phase:1,deny,t:normalizePath,msg:'Path Traversal'"

# Chain multiple decodings for evasion detection
SecRule ARGS "@rx eval\s*\(" \
    "id:103,phase:2,deny,t:urlDecode,t:base64Decode,t:lowercase,msg:'Code Injection'"
```

**Parameter Type:** `string`

**Case Sensitive:** Yes
