+++
title = "AUTH_TYPE"
weight = 38
+++

**描述:** 认证类型


**语法:** `AUTH_TYPE`

**是否实现:** 否

此指令WGE可以正常解析，但是暂未实现该功能。

**示例:**


```apache
SecRule AUTH_TYPE "@rx value" "id:1001,deny,msg:'Test'"
```
