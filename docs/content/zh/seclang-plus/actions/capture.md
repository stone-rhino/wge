+++
title = "capture"
weight = 23
+++

**描述:** 捕获正则表达式匹配的内容

**信息:** Original Example: `capture`

**语法:** `capture | capture:value`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,capture,status:403"
```


**参数类型:** `无`


**区分大小写:** 是
