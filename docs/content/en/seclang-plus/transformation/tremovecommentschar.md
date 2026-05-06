+++
title = "t:removeCommentsChar"
weight = 22
+++

**Description:** Removes comment characters from a string.

This transformation function removes the following comment characters from the input string:
- ``--``
- ``#``
- ``/*``
- ``*/``
- ``<!--``

**Syntax:** `t:removeCommentsChar`

**Implemented:** Yes

**Example:**

```apache
SecRule ARGS "@rx test" "id:1001,t:removeCommentsChar"
```
