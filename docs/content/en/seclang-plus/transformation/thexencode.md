+++
title = "t:hexEncode"
weight = 12
+++

**Description:** Hexadecimal encode

**Syntax:** `t:hexEncode`

Encodes a string (which may contain binary characters) into hexadecimal representation, with two hex characters per input byte. For example, `xyz` would be encoded as `78797a`.

**Example:**

```apache
SecRule ARGS "@rx test" "id:1001,t:hexEncode"
```

**Parameter Type:** `string`
