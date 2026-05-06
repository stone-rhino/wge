+++
title = "t:length"
weight = 33
+++

**Description:** Calculates the length of a string.

**Syntax:** `t:length`

**Implemented:** Yes

**Example:**

```apache
SecRule ARGS "@rx 128" "id:1001,t:length"
```
