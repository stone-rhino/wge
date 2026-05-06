+++
title = "t:removeNulls"
weight = 18
+++

**描述:** 对字符串移除 ``NULL`` 字符

此转换函数会对输入字符串移除所有 ``NULL（0x00）`` 字符。

**语法:** `t:removeNulls`

**是否实现:** 是

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,t:removeNulls"
```



