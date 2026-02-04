+++
title = "ARGS_POST_NAMES"
weight = 6
+++

**描述:** POST 参数名称


**语法:** `ARGS_POST_NAMES`


ARGS_POST_NAMES 仅包含请求体中参数的名称。用于检测 POST 参数名是否包含可疑内容，需要启用 SecRequestBodyAccess 才能访问。


**示例:**


```apache
SecRule ARGS_POST_NAMES "@detectXSS" "id:1008,phase:2,deny,msg:'POST 参数名检测到 XSS'"
```
