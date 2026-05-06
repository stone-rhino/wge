+++
title = "t:urlDecode"
weight = 4
+++

**Description:** Performs ``URL`` decoding on a string.

This transformation function performs ``URL`` decoding on the input string. Invalid encodings will be skipped without affecting the decoding of the remaining valid encodings.

**Syntax:** `t:urlDecode`

**Implemented:** Yes

**Example:**

```apache
SecRule ARGS "@rx test" "id:1001,t:urlDecode"
```
