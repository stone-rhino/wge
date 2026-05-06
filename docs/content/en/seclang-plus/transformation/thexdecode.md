+++
title = "t:hexDecode"
weight = 11
+++

**Description:** Decodes a hexadecimal-encoded string.

**Syntax:** `t:hexDecode`

**Implemented:** Yes

**Example:**

```apache
SecRule ARGS "@rx test" "id:1001,t:hexDecode"
```
