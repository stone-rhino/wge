+++
title = "t:urlDecode"
weight = 4
+++

**描述:** 对字符串进行 ``URL`` 解码。

此转换函数会对输入字符串进行 ``URL`` 解码，非法编码将会跳过但不会影响其余合法编码的解码。

**语法:** `t:urlDecode`

**是否实现:** 是

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,t:urlDecode"
```
