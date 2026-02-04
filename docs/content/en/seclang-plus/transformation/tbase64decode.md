+++
title = "t:base64Decode"
weight = 7
+++

**Description:** Base64 decode

**Syntax:** `t:base64Decode`

The t:base64Decode transformation decodes Base64-encoded strings. Attackers often use Base64 encoding to hide malicious payloads and bypass security detection. This transformation decodes the data before inspection, revealing hidden attack patterns.

**Example:**

```apache
# Detect Base64-encoded attack payloads
SecRule ARGS:data "@rx (eval|exec|system)" \
    "id:1,phase:2,t:base64Decode,deny,msg:'Base64-encoded code execution detected'"

# Original input: ZXZhbChiYXNlNjRfZGVjb2RlKCR4KSk=
# After decoding: eval(base64_decode($x))

# Combine with other transformations for detection
SecRule ARGS "@detectSQLi" \
    "id:2,phase:2,t:base64Decode,t:urlDecode,deny,msg:'Encoded SQL injection'"
```

**Parameter Type:** `none`

**Case Sensitive:** Yes
