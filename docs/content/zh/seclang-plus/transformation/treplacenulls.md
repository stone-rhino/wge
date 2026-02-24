+++
title = "t:replaceNulls"
weight = 24
+++

**描述:** 将字符串里的 ``NULL(0x00)`` 字符替换为空格。

此转换函数会将输入字符串里的所有 ``NULL`` 字符替换为空格字符（``ASCII 0x20``）。

**语法:** `t:replaceNulls`

**是否实现:** 是

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,t:replaceNulls"
```
