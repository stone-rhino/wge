+++
title = "SERVER_ADDR"
weight = 35
+++

**描述:** 服务器 IP 地址


**语法:** `SERVER_ADDR`


**示例:**


```apache
SecRule SERVER_ADDR "@rx value" "id:1001,deny,msg:'Test'"
```


**是否实现:** 暂无
