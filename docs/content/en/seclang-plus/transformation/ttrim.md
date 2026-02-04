+++
title = "t:trim"
weight = 36
+++

**Description:** Trim leading and trailing whitespace

**Syntax:** `t:trim`

Removes whitespace from both the left and right sides of the input string. This combines the functionality of t:trimLeft and t:trimRight.

**Example:**

```apache
SecRule ARGS "@rx test" "id:1001,t:trim"
```

**Parameter Type:** `string`
