+++
title = "t:normalizePathWin"
weight = 27
+++

**Description:** Normalize Windows path

**Syntax:** `t:normalizePathWin`

Functions the same as normalizePath, but first converts backslashes to forward slashes. This is essential for detecting path traversal attacks on Windows systems where backslashes are used as path separators.

**Example:**

```apache
SecRule ARGS "@rx test" "id:1001,t:normalizePathWin"
```

**Parameter Type:** `string`
