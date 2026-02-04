+++
title = "RESPONSE_HEADERS"
weight = 25
+++

**描述:** 响应头


**语法:** `RESPONSE_HEADERS | RESPONSE_HEADERS:Key`


**示例:**


```apache
SecRule RESPONSE_HEADERS:User-Agent "@rx box" "id:1001,deny,msg:'Test'"
```


**是否实现:** 暂无
