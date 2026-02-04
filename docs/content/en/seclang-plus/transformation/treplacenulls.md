+++
title = "t:replaceNulls"
weight = 24
+++

**Description:** Replace null characters

**Syntax:** `t:replaceNulls`

Replaces NUL bytes in the input with space characters (ASCII 0x20). This is useful for detecting attacks that use null bytes to evade string-based detection.

**Example:**

```apache
SecRule ARGS "@rx test" "id:1001,t:replaceNulls"
```

**Parameter Type:** `string`
