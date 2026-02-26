+++
title = "t:hexEncode"
weight = 12
+++

**Description:** Encodes a string using hexadecimal encoding.

**Syntax:** `t:hexEncode`

**Implemented:** Yes

**Example:**

```apache
SecRule ARGS "@rx 74657374" "id:1001,t:hexEncode"
# test encoded with hexEncode becomes 74657374
```
