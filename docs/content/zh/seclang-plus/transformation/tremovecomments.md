+++
title = "t:removeComments"
weight = 21
+++

**描述:** 移除注释

**语法:** `t:removeComments`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,t:removeComments"
```


**参数类型:** `string`


移除所有注释符号（/* ... */、--、#）。连续出现的多个符号不会被压缩。
