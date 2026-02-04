+++
title = "RESPONSE_CONTENT_LENGTH"
weight = 29
+++

**描述:** 响应内容长度


**语法:** `RESPONSE_CONTENT_LENGTH`


**示例:**


```apache
SecRule RESPONSE_CONTENT_LENGTH "@rx value" "id:1001,deny,msg:'Test'"
```


**是否实现:** 暂无
