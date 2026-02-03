+++
title = "t:compressWhitespace"
weight = 20
+++

**Description:** Compress whitespace

**Syntax:** `t:compressWhitespace`

Converts any whitespace characters (0x20, \f, \t, \n, \r, \v, 0xa0) to spaces (ASCII 0x20) and compresses multiple consecutive spaces into a single space. This is useful for normalizing input that may contain various whitespace patterns to evade detection.

**Example:**

```apache
SecRule ARGS "@rx test" "id:1001,t:compressWhitespace"
```

**Parameter Type:** `string`
