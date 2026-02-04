+++
title = "t:parityOdd7bit"
weight = 30
+++

**Description:** 7-bit odd parity decode

**Syntax:** `t:parityOdd7bit`

Decodes data that was encoded with 7-bit odd parity. Removes the parity bit from each byte to recover the original 7-bit data.

**Example:**

```apache
SecRule ARGS "@rx test" "id:1001,t:parityOdd7bit"
```

**Parameter Type:** `string`

**Implementation Status:** Not yet implemented
