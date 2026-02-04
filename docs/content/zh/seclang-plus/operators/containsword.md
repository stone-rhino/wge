+++
title = "@containsWord"
weight = 6
+++

**描述:** 包含指定单词


**语法:** `"@containsWord parameter"`


**示例:**


```apache
SecRule ARGS "@containsWord parameter" "id:1001,deny,msg:'Test'"
```


**参数类型:** `string`


**是否实现:** 暂无
