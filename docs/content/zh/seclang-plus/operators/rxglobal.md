+++
title = "@rxGlobal"
weight = 12
+++

**描述:** 全局正则表达式匹配 (匹配所有出现)


**语法:** `"@rxGlobal parameter"`


**示例:**


```apache
SecRule ARGS "@rxGlobal ^admin" "id:1001,deny,msg:'Test'"
```


**参数类型:** `string (正则)`


**是否实现:** 暂无
