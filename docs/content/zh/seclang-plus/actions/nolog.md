+++
title = "nolog"
weight = 19
+++

**描述:** 不记录到错误日志

**信息:** Original Example: `nolog`

**语法:** `nolog | nolog:value`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,nolog,status:403"
```


**参数类型:** `无`


**区分大小写:** 是
