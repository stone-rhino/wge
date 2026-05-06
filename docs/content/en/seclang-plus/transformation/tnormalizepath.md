+++
title = "t:normalizePath"
weight = 25
+++

**Description:** Normalizes ``Linux`` path in a string.

Same as `t:normalisePath`.

**Syntax:** `t:normalizePath`

**Implemented:** Yes

**Example:**

```apache
SecRule ARGS "@rx test" "id:1001,t:normalizePath"
```
