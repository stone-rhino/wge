+++
title = "t:base64Decode"
weight = 7
+++

**Description:** Decodes a ``Base64``-encoded string.

**Syntax:** `t:base64Decode`

**Implemented:** Yes

**Example:**
```apache
# Detect Base64-encoded attack payloads
SecRule ARGS:data "@rx (eval|exec|system)" \
    "id:1,phase:2,t:base64Decode,deny,msg:'Base64-encoded code execution detected'"

# Original input: ZXZhbChiYXNlNjRfZGVjb2RlKCR4KSk=
# After decoding: eval(base64_decode($x))
```
