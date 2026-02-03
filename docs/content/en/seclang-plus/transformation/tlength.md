+++
title = "t:length"
weight = 33
+++

**Description:** Get string length

**Syntax:** `t:length`

Calculates the length of the input string (in bytes) and outputs the result as a string. For example, given the input `ABCDE`, this transformation will return `5`.

**Example:**

```apache
SecRule ARGS "@gt 100" "id:1001,t:length,deny,msg:'Parameter too long'"
```

**Parameter Type:** `string`
