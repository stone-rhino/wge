+++
title = "t:urlDecodeUni"
weight = 5
+++

**Description:** Unicode URL decode

**Syntax:** `t:urlDecodeUni`

Functions the same as urlDecode but adds support for Microsoft's proprietary %u encoding. If the code value is in the range FF01-FF5E (fullwidth ASCII), the high byte is used to detect and adjust the low byte; otherwise only the low byte is used and the high byte is zeroed.

**Example:**

```apache
SecRule ARGS "@rx test" "id:1001,t:urlDecodeUni"
```

**Parameter Type:** `string`
