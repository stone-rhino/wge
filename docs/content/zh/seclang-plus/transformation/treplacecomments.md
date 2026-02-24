+++
title = "t:replaceComments"
weight = 23
+++

**描述:** 将字符串里的注释块替换为空格。

此转换函数会将输入字符串里由 ``/**/`` 包裹的注释块替换为空格。不完整的注释也会被替换为一个空格。但是，独立的注释终止符(``*/``) 将不会受到影响。

**语法:** `t:replaceComments`

**是否实现:** 是

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,t:replaceComments"
```

