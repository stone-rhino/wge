+++
title = "t:normalisePathWin"
weight = 28
+++

**Description:** Normalize Windows path (British spelling)

**Syntax:** `t:normalisePathWin`

Alias for normalizePathWin. See normalizePathWin for details.

**Example:**

```apache
SecRule ARGS "@rx test" "id:1001,t:normalisePathWin"
```

**Parameter Type:** `string`
