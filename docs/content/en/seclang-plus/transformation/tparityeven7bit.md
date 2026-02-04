+++
title = "t:parityEven7bit"
weight = 29
+++

**Description:** 7-bit even parity decode

**Syntax:** `t:parityEven7bit`

Decodes data that was encoded with 7-bit even parity. Removes the parity bit from each byte to recover the original 7-bit data.

**Example:**

```apache
SecRule ARGS "@rx test" "id:1001,t:parityEven7bit"
```

**Parameter Type:** `string`

**Implementation Status:** Not yet implemented
