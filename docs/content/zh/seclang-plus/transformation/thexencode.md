+++
title = "t:hexEncode"
weight = 12
+++

**描述:** 对字符串进行十六进制编码。

**语法:** `t:hexEncode`

**是否实现:** 是

**示例:**

```apache
SecRule ARGS "@rx 74657374" "id:1001,t:hexEncode"
# test 经 hexEncode 后为 74657374
```