+++
title = "t:removeComments"
weight = 21
+++

**Description:** Removes comment blocks from a string.

This transformation function removes the following comment blocks from the input string:
- ``/**/``
- ``<!---->``
- ``--``
- ``#``

**Syntax:** `t:removeComments`

**Implemented:** Yes

**Example:**

```apache
SecRule ARGS "@rx test" "id:1001,t:removeComments"
```
