+++
title = "t:removeNulls"
weight = 18
+++

**Description:** Removes ``NULL`` characters from a string.

This transformation function removes all ``NULL (0x00)`` characters from the input string.

**Syntax:** `t:removeNulls`

**Implemented:** Yes

**Example:**

```apache
SecRule ARGS "@rx test" "id:1001,t:removeNulls"
```
