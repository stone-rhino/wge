+++
title = "RESPONSE_HEADERS_NAMES"
weight = 26
+++

**描述:** 响应头名称


**语法:** `RESPONSE_HEADERS_NAMES | RESPONSE_HEADERS_NAMES:Key`


**示例:**


```apache
SecRule RESPONSE_HEADERS_NAMES:User-Agent "@rx box" "id:1001,deny,msg:'Test'"
```


**是否实现:** 暂无
