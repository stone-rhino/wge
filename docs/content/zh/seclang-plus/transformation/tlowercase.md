+++
title = "t:lowercase"
weight = 2
+++

**描述:** 对字符串进行大写转小写。

**语法:** `t:lowercase`

**是否实现:** 是

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,t:lowercase"
```
