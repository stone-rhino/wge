+++
title = "t:replaceNulls"
weight = 24
+++

**Description:** Replaces ``NULL (0x00)`` characters in a string with spaces.

This transformation function replaces all ``NULL`` characters in the input string with space characters (``ASCII 0x20``).

**Syntax:** `t:replaceNulls`

**Implemented:** Yes

**Example:**

```apache
SecRule ARGS "@rx test" "id:1001,t:replaceNulls"
```
