+++
title = "REQUEST_BODY_LENGTH"
weight = 13
+++

**描述:** 请求体长度


**语法:** `REQUEST_BODY_LENGTH`


**示例:**


```apache
SecRule REQUEST_BODY_LENGTH "@rx value" "id:1001,deny,msg:'Test'"
```


**是否实现:** 暂无
