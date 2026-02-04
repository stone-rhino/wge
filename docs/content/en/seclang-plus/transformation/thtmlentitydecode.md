+++
title = "t:htmlEntityDecode"
weight = 13
+++

**Description:** HTML entity decode

**Syntax:** `t:htmlEntityDecode`

Decodes characters encoded as HTML entities. Supports various HTML entity formats including named entities, decimal numeric entities, and hexadecimal numeric entities. The function always converts a single HTML entity to one byte, which may cause information loss if the entity represents a character that cannot be represented in a single byte. It helps reveal bytes that did not need to be encoded, but cannot effectively handle characters above the 0xff range.

**Example:**

```apache
SecRule ARGS "@rx test" "id:1001,t:htmlEntityDecode"
```

**Parameter Type:** `string`
