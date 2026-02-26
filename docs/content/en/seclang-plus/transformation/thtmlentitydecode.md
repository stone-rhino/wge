+++
title = "t:htmlEntityDecode"
weight = 13
+++

**Description:** Decodes ``HTML entity`` escape sequences in a string.

**Syntax:** `t:htmlEntityDecode`

**Implemented:** Yes

**Example:**

```apache
SecRule ARGS "@rx test" "id:1001,t:htmlEntityDecode"
```
