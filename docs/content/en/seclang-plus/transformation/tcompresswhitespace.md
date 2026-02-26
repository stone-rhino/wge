+++
title = "t:compressWhitespace"
weight = 20
+++

**Description:** Compresses whitespace in a string, reducing consecutive spaces to a single space.

This transformation function processes the input string in the following ways:
 - Converts all whitespace characters (including ``0x20, \f, \t, \n, \r, \v, 0xa0``) to spaces
 - Compresses consecutive spaces into a single space

**Syntax:** `t:compressWhitespace`

**Implemented:** Yes

**Example:**

```apache
SecRule ARGS "@rx test" "id:1001,t:compressWhitespace"
```
