+++
title = "@rbl"
weight = 36
+++

**描述:** 实时黑名单查询


**语法:** `"@rbl parameter"`


**示例:**


```apache
SecRule ARGS "@rbl parameter" "id:1001,deny,msg:'Test'"
```


**参数类型:** `string`


**是否实现:** 暂无
