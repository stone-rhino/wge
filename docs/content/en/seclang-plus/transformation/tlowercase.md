+++
title = "t:lowercase"
weight = 2
+++

**Description:** Convert to lowercase

**Syntax:** `t:lowercase`

Converts all characters to lowercase using the current C locale settings. This is one of the most commonly used transformations for case-insensitive matching.

**Example:**

```apache
SecRule ARGS "@rx test" "id:1001,t:lowercase"
```

**Parameter Type:** `string`
