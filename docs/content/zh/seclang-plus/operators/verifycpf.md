+++
title = "@verifyCpf"
weight = 27
+++

**描述:** 验证巴西 CPF 号码


**语法:** `"@verifyCpf parameter"`


**示例:**


```apache
SecRule ARGS "@verifyCpf parameter" "id:1001,deny,msg:'Test'"
```


**参数类型:** `string (正则)`


**是否实现:** 暂无
