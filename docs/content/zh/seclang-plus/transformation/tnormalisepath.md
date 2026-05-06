+++
title = "t:normalisePath"
weight = 26
+++

**描述:** 对字符串进行 ``Linux`` 路径规范化。

此转换函数会对输入字符串进行规范化，去除连续的斜杠、当前目录引用（``.``）以及上级目录引用（``..``），但位于字符串开头的除外。

**语法:** `t:normalisePath`

**是否实现:** 是

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,t:normalisePath"
```
