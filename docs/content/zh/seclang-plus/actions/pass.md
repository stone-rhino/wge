+++
title = "pass"
weight = 16
+++

**描述:** 继续处理后续规则

**信息:** Original Example: `pass`

**语法:** `pass | pass:value`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,pass,status:403"
```


**参数类型:** `无`


**区分大小写:** 是
