+++
title = "t:normalisePath"
weight = 26
+++

**Description:** Normalize path (British spelling)

**Syntax:** `t:normalisePath`

Alias for normalizePath. See normalizePath for details.

**Example:**

```apache
SecRule ARGS "@rx test" "id:1001,t:normalisePath"
```

**Parameter Type:** `string`
