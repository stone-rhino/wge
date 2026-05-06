+++
title = "t:removeComments"
weight = 21
+++

**描述:** 对字符串移除注释块。

此转换函数会对输入字符串移除下列注释块：
- ``/**/``
- ``<!---->``
- ``--``
- ``#``

**语法:** `t:removeComments`  

**是否实现:** 是

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,t:removeComments"
```
