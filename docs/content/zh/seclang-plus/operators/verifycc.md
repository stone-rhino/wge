+++
title = "@verifyCc"
weight = 26
+++

**描述:** 验证信用卡号


**语法:** `"@verifyCc parameter"`


**示例:**


```apache
SecRule ARGS "@verifyCc parameter" "id:1001,deny,msg:'Test'"
```


**参数类型:** `string (正则)`


**是否实现:** 暂无
