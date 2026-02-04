+++
title = "setuid"
weight = 28
+++

**描述:** 设置用户 ID

**信息:** Original Example: `setuid:%{REMOTE_ADDR}`

**语法:** `setuid | setuid:value`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,setuid,status:403"
```


**参数类型:** `string`


**是否实现:** 暂无


**区分大小写:** 是
