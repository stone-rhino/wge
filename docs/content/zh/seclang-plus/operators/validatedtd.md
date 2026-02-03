+++
title = "@validateDtd"
weight = 24
+++

**描述:** 根据 DTD 验证 XML


**语法:** `"@validateDtd parameter"`


**示例:**


```apache
SecRule ARGS "@validateDtd parameter" "id:1001,deny,msg:'Test'"
```


**参数类型:** `string`


**是否实现:** 暂无
