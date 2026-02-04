+++
title = "t:replaceComments"
weight = 23
+++

**Description:** Replace comments with space

**Syntax:** `t:replaceComments`

Replaces all C-style comments (/* ... */) with a single space (consecutive spaces are not compressed). Unterminated comments are also replaced with a space (ASCII 0x20). However, standalone comment terminators (*/) are not processed.

**Example:**

```apache
SecRule ARGS "@rx test" "id:1001,t:replaceComments"
```

**Parameter Type:** `string`
