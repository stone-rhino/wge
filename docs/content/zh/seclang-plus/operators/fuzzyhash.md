+++
title = "@fuzzyHash"
weight = 34
+++

**描述:** 模糊哈希匹配


**语法:** `"@fuzzyHash parameter"`


**示例:**


```apache
SecRule ARGS "@fuzzyHash parameter" "id:1001,deny,msg:'Test'"
```


**参数类型:** `string`


**是否实现:** 暂无
