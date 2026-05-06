+++
title = "t:jsDecode"
weight = 14
+++

**Description:** Decodes ``JavaScript`` escape sequences in a string.

If the ``\uHHHH`` code point is in the ``FF01-FF5E`` range (fullwidth ASCII), the high byte is used to detect and adjust the low byte. Otherwise, only the low byte is used and the high byte is zeroed (which may result in information loss).

**Syntax:** `t:jsDecode`

**Implemented:** Yes

**Example:**

```apache
SecRule ARGS "@rx test" "id:1001,t:jsDecode"
```
