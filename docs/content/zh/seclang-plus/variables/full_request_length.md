+++
title = "FULL_REQUEST_LENGTH"
weight = 23
+++

**描述:** 完整请求的长度


**语法:** `FULL_REQUEST_LENGTH`


**示例:**


```apache
SecRule FULL_REQUEST_LENGTH "@rx value" "id:1001,deny,msg:'Test'"
```


**是否实现:** 暂无
