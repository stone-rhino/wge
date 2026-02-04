+++
title = "noauditlog"
weight = 21
+++

**描述:** 不记录到审计日志

**信息:** Original Example: `noauditlog`

**语法:** `noauditlog | noauditlog:value`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,noauditlog,status:403"
```


**参数类型:** `无`


**区分大小写:** 是
