+++
title = "REMOTE_PORT"
weight = 33
+++

**描述:** 客户端端口


**语法:** `REMOTE_PORT`


**示例:**


```apache
SecRule REMOTE_PORT "@rx value" "id:1001,deny,msg:'Test'"
```


**是否实现:** 暂无
