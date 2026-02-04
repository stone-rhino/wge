+++
title = "block"
weight = 17
+++

**描述:** 使用默认破坏性动作

**信息:** Original Example: `block`

**语法:** `block | block:value`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,block,status:403"
```


**参数类型:** `无`


**区分大小写:** 是
