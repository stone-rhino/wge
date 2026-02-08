+++
title = "SERVER_PORT"
weight = 37
+++

**描述:** 服务器端口


**语法:** `SERVER_PORT`


**是否实现:** 否

**示例:**


```apache
SecRule SERVER_PORT "@rx value" "id:1001,deny,msg:'Test'"
```
