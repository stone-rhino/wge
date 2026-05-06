+++
title = "t:trim"
weight = 36
+++

**描述:** 对字符串去除首尾空白。

此转换函数会将输入字符串首尾部的 ``\x20\t\n\r\f\v`` 字符去除。

**语法:** `t:trim`

**是否实现:** 是

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,t:trim"
```



