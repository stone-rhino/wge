+++
title = "expirevar"
weight = 26
+++

**描述:** 设置变量过期时间

**信息:** Original Example: `expirevar:ip.blocked=600`

**语法:** `expirevar | expirevar:value`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,expirevar,status:403"
```


**参数类型:** `string`


**区分大小写:** 是
