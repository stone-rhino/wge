+++
title = "t:normalizePath"
weight = 25
+++

**Description:** Normalize path

**Syntax:** `t:normalizePath`

Removes multiple slashes, directory self-references (./), and directory back-references (../) from the input string, except when located at the beginning of the input. This is essential for detecting path traversal attacks that use various encoding techniques.

**Example:**

```apache
SecRule ARGS "@rx test" "id:1001,t:normalizePath"
```

**Parameter Type:** `string`
