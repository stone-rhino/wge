+++
title = "SERVER_NAME"
weight = 36
+++

**描述:** 服务器名称


**语法:** `SERVER_NAME`


**示例:**


```apache
SecRule SERVER_NAME "@rx value" "id:1001,deny,msg:'Test'"
```


**是否实现:** 暂无
