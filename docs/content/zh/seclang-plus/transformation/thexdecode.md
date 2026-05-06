+++
title = "t:hexDecode"
weight = 11
+++

**描述:** 对字符串进行十六进制解码。

**语法:** `t:hexDecode`

**是否实现:** 是

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,t:hexDecode"
```
