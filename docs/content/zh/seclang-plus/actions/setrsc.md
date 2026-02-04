+++
title = "setrsc"
weight = 30
+++

**描述:** 设置资源 ID

**信息:** Original Example: `setrsc:%{REQUEST_FILENAME}`

**语法:** `setrsc | setrsc:value`

**示例:**

```apache
SecRule ARGS "@rx test" "id:1001,setrsc,status:403"
```


**参数类型:** `string`


**是否实现:** 暂无


**区分大小写:** 是
