+++
title = "setsid"
weight = 29
+++

**描述:** 设置会话 ID

**信息:** Original Example: `setsid:%{SESSION.id}`

**语法:** `setsid | setsid:value`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,setsid,status:403"
```


**参数类型:** `string`


**是否实现:** 暂无


**区分大小写:** 是
