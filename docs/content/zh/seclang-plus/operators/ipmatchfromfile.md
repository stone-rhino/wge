+++
title = "@ipMatchFromFile"
weight = 20
+++

**描述:** 从文件加载 IP 地址列表匹配


**语法:** `"@ipMatchFromFile parameter"`


**示例:**


```apache
SecRule ARGS "@ipMatchFromFile 192.168.1.1" "id:1001,deny,msg:'Test'"
```


**参数类型:** `string (文件路径)`


**是否实现:** 暂无
