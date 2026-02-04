+++
title = "t:base64Encode"
weight = 9
+++

**Description:** Base64 encode

**Syntax:** `t:base64Encode`

Encodes the input string using Base64 encoding. This is useful for comparing against known Base64-encoded patterns or for logging purposes.

**Example:**

```apache
SecRule ARGS "@rx test" "id:1001,t:base64Encode"
```

**Parameter Type:** `string`

**Implementation Status:** Not yet implemented
