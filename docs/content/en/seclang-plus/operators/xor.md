+++
title = "@xor"
weight = 13
+++

**Description:** Numeric XOR

**Syntax:** `@xor string`

**Input Data Type:** `int`

`@xor` performs bitwise XOR on the variable value and the parameter. A non-zero XOR result is treated as match. When macro expansion is not used for the parameter, parsing follows the same prefix-parse behavior as `@eq`.

**Example:**

```apache
# Content-Length and Transfer-Encoding must not coexist
SecRule &REQUEST_HEADERS:Content-Length "!@xor %{&REQUEST_HEADERS:Transfer-Encoding}" \
    "id:1085,phase:3,pass,log,msg:'Content-Length and Transfer-Encoding both exist'"
```
