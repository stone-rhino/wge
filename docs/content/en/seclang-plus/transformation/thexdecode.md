+++
title = "t:hexDecode"
weight = 11
+++

**Description:** Hexadecimal decode

**Syntax:** `t:hexDecode`

Decodes a string that was encoded using the same algorithm as hexEncode. Converts pairs of hexadecimal characters back to their original byte values.

**Example:**

```apache
SecRule ARGS "@rx test" "id:1001,t:hexDecode"
```

**Parameter Type:** `string`
