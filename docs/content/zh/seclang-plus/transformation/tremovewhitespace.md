+++
title = "t:removeWhitespace"
weight = 19
+++

**描述:** 对字符串移除空白字符。

此转换函数会对输入字符串移除所有 ``\x20\f\t\n\r\v\xA0`` 字符。

**语法:** `t:removeWhitespace`

**是否实现:** 是

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,t:removeWhitespace"
```