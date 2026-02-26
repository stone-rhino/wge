+++
title = "t:replaceComments"
weight = 23
+++

**Description:** Replaces comment blocks in a string with spaces.

This transformation function replaces comment blocks enclosed by ``/**/`` in the input string with spaces. Incomplete comments are also replaced with a single space. However, standalone comment terminators (``*/``) are not affected.

**Syntax:** `t:replaceComments`

**Implemented:** Yes

**Example:**

```apache
SecRule ARGS "@rx test" "id:1001,t:replaceComments"
```
