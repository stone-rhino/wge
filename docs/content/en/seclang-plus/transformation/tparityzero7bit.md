+++
title = "t:parityZero7bit"
weight = 31
+++

**Description:** 7-bit zero parity decode

**Syntax:** `t:parityZero7bit`

Decodes data that was encoded with 7-bit zero parity. Removes the parity bit (which is always zero) from each byte to recover the original 7-bit data.

**Example:**

```apache
SecRule ARGS "@rx test" "id:1001,t:parityZero7bit"
```

**Parameter Type:** `string`

**Implementation Status:** Not yet implemented
