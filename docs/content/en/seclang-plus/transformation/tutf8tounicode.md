+++
title = "t:utf8ToUnicode"
weight = 32
+++

**Description:** Performs ``Unicode`` encoding on ``UTF-8`` character sequences in a string.

This transformation function converts all ``UTF-8`` character sequences in the input string to ``Unicode`` (using ``%uHHHH`` format).

**Syntax:** `t:utf8ToUnicode`

**Implemented:** Yes

**Example:**

```apache
SecRule ARGS "@rx test" "id:1001,t:utf8ToUnicode"
```
