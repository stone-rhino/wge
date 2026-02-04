+++
title = "@verifySsn"
weight = 28
+++

**描述:** 验证美国社会安全号


**语法:** `"@verifySsn parameter"`


**示例:**


```apache
SecRule ARGS "@verifySsn parameter" "id:1001,deny,msg:'Test'"
```


**参数类型:** `string (正则)`


**是否实现:** 暂无
