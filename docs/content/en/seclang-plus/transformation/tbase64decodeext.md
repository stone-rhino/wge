+++
title = "t:base64DecodeExt"
weight = 8
+++

**Description:** Extended Base64 decode (supports more variants)

**Syntax:** `t:base64DecodeExt`

Decodes Base64-encoded strings with extended support for non-standard Base64 variants. This is more permissive than t:base64Decode and can handle malformed or variant Base64 encodings that attackers might use to evade detection.

**Example:**

```apache
SecRule ARGS "@rx test" "id:1001,t:base64DecodeExt"
```

**Parameter Type:** `string`

**Implementation Status:** Not yet implemented
