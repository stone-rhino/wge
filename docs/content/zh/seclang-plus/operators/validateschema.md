+++
title = "@validateSchema"
weight = 25
+++

**描述:** 根据 Schema 验证 XML


**语法:** `"@validateSchema parameter"`


**示例:**


```apache
SecRule ARGS "@validateSchema parameter" "id:1001,deny,msg:'Test'"
```


**参数类型:** `string`


**是否实现:** 暂无
