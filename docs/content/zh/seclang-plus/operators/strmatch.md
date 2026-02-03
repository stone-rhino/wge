+++
title = "@strmatch"
weight = 8
+++

**描述:** 通配符匹配


**语法:** `"@strmatch parameter"`


**示例:**


```apache
SecRule ARGS "@strmatch parameter" "id:1001,deny,msg:'Test'"
```


**参数类型:** `string (通配符)`


**是否实现:** 暂无
