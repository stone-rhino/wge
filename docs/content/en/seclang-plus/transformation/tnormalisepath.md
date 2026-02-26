+++
title = "t:normalisePath"
weight = 26
+++

**Description:** Normalizes ``Linux`` path in a string.

This transformation function normalizes the input string by removing consecutive slashes, current directory references (``.``), and parent directory references (``..``), except when located at the beginning of the string.

**Syntax:** `t:normalisePath`

**Implemented:** Yes

**Example:**

```apache
SecRule ARGS "@rx test" "id:1001,t:normalisePath"
```
