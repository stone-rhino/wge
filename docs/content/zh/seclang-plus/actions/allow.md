+++
title = "allow"
weight = 12
+++

**描述:** 允许请求通过

**信息:** Original Example: `allow`

**语法:** `allow | allow:value`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,allow,status:403"
```


**参数类型:** `无 或 string`


**区分大小写:** 是
