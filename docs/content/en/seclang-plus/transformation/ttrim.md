+++
title = "t:trim"
weight = 36
+++

**Description:** Removes leading and trailing whitespace from a string.

This transformation function removes ``\x20\t\n\r\f\v`` characters from the beginning and end of the input string.

**Syntax:** `t:trim`

**Implemented:** Yes

**Example:**

```apache
SecRule ARGS "@rx test" "id:1001,t:trim"
```
