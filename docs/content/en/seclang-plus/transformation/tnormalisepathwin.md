+++
title = "t:normalisePathWin"
weight = 28
+++

**Description:** Normalizes ``Windows`` path in a string.

This transformation function is similar to `t:normalisePath`, but first converts all backslashes ``\`` to forward slashes ``/``.

**Syntax:** `t:normalisePathWin`

**Implemented:** Yes

**Example:**

```apache
SecRule ARGS "@rx test" "id:1001,t:normalisePathWin"
```
