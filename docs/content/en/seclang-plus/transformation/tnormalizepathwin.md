+++
title = "t:normalizePathWin"
weight = 27
+++

**Description:** Normalizes ``Linux`` path in a string.

Same as `t:normalisePathWin`.

**Syntax:** `t:normalizePathWin`

**Implemented:** Yes

**Example:**

```apache
SecRule ARGS "@rx test" "id:1001,t:normalizePathWin"
```
