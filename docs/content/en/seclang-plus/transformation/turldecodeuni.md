+++
title = "t:urlDecodeUni"
weight = 5
+++

**Description:** Performs ``URL`` decoding on a string.

This transformation function is similar to ``t:urlDecode``, but adds support for ``Microsoft``-specific ``%u`` encoding. If the code point is in the ``FF01-FF5E`` range (fullwidth ``ASCII``), the high byte is used to detect and adjust the low byte. Otherwise, only the low byte is used and the high byte is zeroed.

**Syntax:** `t:urlDecodeUni`

**Implemented:** Yes

**Example:**

```apache
SecRule ARGS "@rx test" "id:1001,t:urlDecodeUni"
```
