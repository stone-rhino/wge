+++
title = "t:removeCommentsChar"
weight = 22
+++

**描述:** 对字符串移除注释字符。

此转换函数会对输入字符串移除下列注释字符：
- ``--``
- ``#``
- ``/*``
- ``*/``
- ``<!--``

**语法:** `t:removeCommentsChar`

**是否实现:** 是

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,t:removeCommentsChar"
```
