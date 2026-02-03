+++
title = "multiMatch"
weight = 24
+++

**描述:** 对所有匹配执行动作

**信息:** Original Example: `multiMatch`

**语法:** `multiMatch | multiMatch:value`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,multiMatch,status:403"
```


**参数类型:** `无`


**区分大小写:** 是
