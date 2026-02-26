+++
title = "t:removeWhitespace"
weight = 19
+++

**Description:** Removes whitespace characters from a string.

This transformation function removes all ``\x20\f\t\n\r\v\xA0`` characters from the input string.

**Syntax:** `t:removeWhitespace`

**Implemented:** Yes

**Example:**

```apache
SecRule ARGS "@rx test" "id:1001,t:removeWhitespace"
```
