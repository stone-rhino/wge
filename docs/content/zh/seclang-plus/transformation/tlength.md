+++
title = "t:length"
weight = 33
+++

**描述:** 对字符串计算长度。

**语法:** `t:length`

**是否实现:** 是

**示例:**

```apache
SecRule ARGS "@rx 128" "id:1001,t:length"
```
