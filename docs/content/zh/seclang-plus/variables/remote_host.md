+++
title = "REMOTE_HOST"
weight = 32
+++

**描述:** 客户端主机名


**语法:** `REMOTE_HOST`


**示例:**


```apache
SecRule REMOTE_HOST "@rx value" "id:1001,deny,msg:'Test'"
```


**是否实现:** 暂无
