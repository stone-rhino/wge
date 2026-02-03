+++
title = "auditlog"
weight = 20
+++

**描述:** 记录到审计日志

**信息:** Original Example: `auditlog`

**语法:** `auditlog | auditlog:value`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,auditlog,status:403"
```


**参数类型:** `无`


**区分大小写:** 是
