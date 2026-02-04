+++
title = "t:sqlHexDecode"
weight = 17
+++

**Description:** SQL hexadecimal decode

**Syntax:** `t:sqlHexDecode`

Decodes SQL hex-encoded strings (e.g., 0x414243 to ABC). This is useful for detecting SQL injection attacks that use hexadecimal encoding to obfuscate malicious payloads.

**Example:**

```apache
SecRule ARGS "@rx test" "id:1001,t:sqlHexDecode"
```

**Parameter Type:** `string`

**Implementation Status:** Not yet implemented
